/*
 * Copyright (c) 1995-2003 by Radford M. Neal
 *
 * Permission is granted for anyone to copy, use, modify, or distribute these
 * programs and accompanying documents for any purpose, provided this copyright
 * notice is retained and prominently displayed, along with a note saying
 * that the original programs are available from Radford Neal's web page, and
 * note is made of any changes made to these programs.  These programs and
 * documents are distributed without any warranty, express or implied.  As the
 * programs were written for research purposes only, they have not been tested
 * to the degree that would be advisable in any important application.  All use
 * of these programs is entirely at the user's own risk.
 */

// MOD2SPARSE.C - Procedures for handling sparse mod2 matrices.

// NOTE:  See mod2sparse.html for documentation on these procedures.

#include "ldpc_matrix_sparse.h"

#include <stdio.h>
#include <stdlib.h>

#include "tools.h"

// ALLOCATE AN ENTRY WITHIN A MATRIX.  This local procedure is used to
// allocate a new entry, representing a non-zero element, within a given
// matrix.  Entries in this matrix that were previously allocated and
// then freed are re-used.  If there are no such entries, a new block
// of entries is allocated.

static mod2entry* alloc_entry(mod2sparse* m) {
  mod2block* b;
  mod2entry* e;
  int k;

  if (m->next_free == 0) {
    b = (mod2block*)chk_alloc(1, sizeof *b);

    b->next = m->blocks;
    m->blocks = b;

    for (k = 0; k < Mod2sparse_block; k++) {
      b->entry[k].left = m->next_free;
      m->next_free = &b->entry[k];
    }
  }

  e = m->next_free;
  m->next_free = e->left;

  return e;
}

// ALLOCATE SPACE FOR A SPARSE MOD2 MATRIX.

mod2sparse* mod2sparse_allocate(int n_rows,  // Number of rows in matrix
                                int n_cols  // Number of columns in matrix
) {
  mod2sparse* m;
  mod2entry* e;
  int i;
  int j;

  if (n_rows <= 0 || n_cols <= 0) {
    fprintf(stderr,
            "mod2sparse_allocate: Invalid number of rows (%d) or columns "
            "(%d)\nBoth values must be > 0.\n",
            n_rows, n_cols);
    exit(1);
  }

  m = (mod2sparse*)chk_alloc(1, sizeof *m);

  m->n_rows = n_rows;
  m->n_cols = n_cols;

  m->rows = (mod2entry*)chk_alloc(n_rows, sizeof *m->rows);
  m->cols = (mod2entry*)chk_alloc(n_cols, sizeof *m->cols);

  m->blocks = 0;
  m->next_free = 0;

  for (i = 0; i < n_rows; i++) {
    e = &m->rows[i];
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
    e->left = e->right = e->up = e->down = e;
#else
    e->left = e->right = e->down = e;
#endif
    e->row = e->col = -1;
  }

  for (j = 0; j < n_cols; j++) {
    e = &m->cols[j];
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
    e->left = e->right = e->up = e->down = e;
#else
    e->left = e->right = e->down = e;
#endif
    e->row = e->col = -1;
  }

  return m;
}

// FREE SPACE OCCUPIED BY A SPARSE MOD2 MATRIX.

void mod2sparse_free(mod2sparse* m  // Matrix to free
) {
  mod2block* b;

  free(m->rows);
  free(m->cols);

  while (m->blocks != 0) {
    b = m->blocks;
    m->blocks = b->next;
    free(b);
  }
}

// CLEAR A SPARSE MATRIX TO ALL ZEROS.

void mod2sparse_clear(mod2sparse* r) {
  mod2block* b;
  mod2entry* e;
  int i;
  int j;

  for (i = 0; i < mod2sparse_rows(r); i++) {
    e = &r->rows[i];
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
    e->left = e->right = e->up = e->down = e;
#else
    e->left = e->right = e->down = e;
#endif
  }

  for (j = 0; j < mod2sparse_cols(r); j++) {
    e = &r->cols[j];
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
    e->left = e->right = e->up = e->down = e;
#else
    e->left = e->right = e->down = e;
#endif
  }

  while (r->blocks != 0) {
    b = r->blocks;
    r->blocks = b->next;
    free(b);
  }
}

// PRINT A SPARSE MOD2 MATRIX IN HUMAN-READABLE FORM.

void mod2sparse_print(FILE* f, mod2sparse* m) {
  int rdigits;
  int cdigits;
  mod2entry* e;
  int i;

  rdigits = mod2sparse_rows(m) <= 10 ? 1
                                     : mod2sparse_rows(m) <= 100
          ? 2
          : mod2sparse_rows(m) <= 1000 ? 3
                                       : mod2sparse_rows(m) <= 10000
                  ? 4
                  : mod2sparse_rows(m) <= 100000 ? 5 : 6;

  cdigits = mod2sparse_cols(m) <= 10 ? 1
                                     : mod2sparse_cols(m) <= 100
          ? 2
          : mod2sparse_cols(m) <= 1000 ? 3
                                       : mod2sparse_cols(m) <= 10000
                  ? 4
                  : mod2sparse_cols(m) <= 100000 ? 5 : 6;

  for (i = 0; i < mod2sparse_rows(m); i++) {
    fprintf(f, "%*d:", rdigits, i);

    e = mod2sparse_first_in_row(m, i);
    while (!mod2sparse_at_end(e)) {
      fprintf(f, " %*d", cdigits, mod2sparse_col(e));
      e = mod2sparse_next_in_row(e);
    }

    fprintf(f, "\n");
  }
}

// LOOK FOR AN ENTRY WITH GIVEN ROW AND COLUMN.

mod2entry* mod2sparse_find(mod2sparse* m, int row, int col) {
  mod2entry* re;
  mod2entry* ce;

  if (row < 0 || row >= mod2sparse_rows(m) || col < 0 ||
      col >= mod2sparse_cols(m)) {
    fprintf(stderr, "mod2sparse_find: row or column index out of bounds\n");
    exit(1);
  }

  // Check last entries in row.

  re = mod2sparse_last_in_row(m, row);
  if (mod2sparse_at_end(re) || mod2sparse_col(re) < col) {
    return 0;
  }
  if (mod2sparse_col(re) == col) {
    return re;
  }

#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  ce = mod2sparse_last_in_col(m, col);
  if (mod2sparse_at_end(ce) || mod2sparse_row(ce) < row) {
    return 0;
  }
  if (mod2sparse_row(ce) == row) {
    return ce;
  }
#endif

  // Search row and column in parallel, from the front.

  re = mod2sparse_first_in_row(m, row);
  ce = mod2sparse_first_in_col(m, col);

  for (;;) {
    if (mod2sparse_at_end(re) || mod2sparse_col(re) > col) {
      return 0;
    }
    if (mod2sparse_col(re) == col) {
      return re;
    }

    if (mod2sparse_at_end(ce) || mod2sparse_row(ce) > row) {
      return 0;
    }
    if (mod2sparse_row(ce) == row) {
      return ce;
    }

    re = mod2sparse_next_in_row(re);
    ce = mod2sparse_next_in_col(ce);
  }
}

// INSERT AN ENTRY WITH GIVEN ROW AND COLUMN.

mod2entry* mod2sparse_insert(mod2sparse* m, int row, int col) {
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  mod2entry* re;
  mod2entry* ce;
  mod2entry* ne;
#else
  mod2entry *re, *ce, *ne, *ce2;
#endif

  if (row < 0 || row >= mod2sparse_rows(m) || col < 0 ||
      col >= mod2sparse_cols(m)) {
    fprintf(stderr, "mod2sparse_insert: row or column index out of bounds\n");
    exit(1);
  }

  // Find old entry and return it, or allocate new entry and insert into row.

  re = mod2sparse_last_in_row(m, row);

  if (!mod2sparse_at_end(re) && mod2sparse_col(re) == col) {
    return re;
  }

  if (mod2sparse_at_end(re) || mod2sparse_col(re) < col) {
    re = re->right;
  } else {
    re = mod2sparse_first_in_row(m, row);

    for (;;) {
      if (!mod2sparse_at_end(re) && mod2sparse_col(re) == col) {
        return re;
      }

      if (mod2sparse_at_end(re) || mod2sparse_col(re) > col) {
        break;
      }

      re = mod2sparse_next_in_row(re);
    }
  }

  ne = alloc_entry(m);

  ne->row = row;
  ne->col = col;

  ne->left = re->left;
  ne->right = re;
  ne->left->right = ne;
  ne->right->left = ne;

  // Insert new entry into column.

#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  // If we find an existing entry here,
  // the matrix must be garbled, since we didn't find it in the row.

  ce = mod2sparse_last_in_col(m, col);

  if (!mod2sparse_at_end(ce) && mod2sparse_row(ce) == row) {
    fprintf(stderr, "mod2sparse_insert: Garbled matrix\n");
    exit(1);
  }

  if (mod2sparse_at_end(ce) || mod2sparse_row(ce) < row) {
    ce = ce->down;
  } else {
#else
  ce2 = &(m->cols[col]);
#endif
    ce = mod2sparse_first_in_col(m, col);

    for (;;) {
      if (!mod2sparse_at_end(ce) && mod2sparse_row(ce) == row) {
        fprintf(stderr, "mod2sparse_insert: Garbled matrix\n");
        exit(1);
      }

      if (mod2sparse_at_end(ce) || mod2sparse_row(ce) > row) {
        break;
      }
#ifdef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
      ce2 = ce;
#endif
      ce = mod2sparse_next_in_col(ce);
    }

#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  }

  ne->up = ce->up;
  ne->down = ce;
  ne->up->down = ne;
  ne->down->up = ne;
#else
  ne->down = ce;
  ce2->down = ne;
#endif

  // Return the new entry.

  return ne;
}

// DELETE AN ENTRY FROM A SPARSE MATRIX.

void mod2sparse_delete(mod2sparse* m, mod2entry* e) {
#ifdef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  mod2entry* ce;
#endif

  if (e == 0) {
    fprintf(stderr, "mod2sparse_delete: Trying to delete a null entry\n");
    exit(1);
  }

  if (e->row < 0 || e->col < 0) {
    fprintf(
        stderr,
        "mod2sparse_delete: Trying to delete a header entry (row=%d, col=%d)\n",
        e->row, e->col);
    exit(1);
  }
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  e->up->down = e->down;
  e->down->up = e->up;

#else
  ce = &(m->cols[e->col]);
  while (ce->down != e) {
    ce = ce->down;
  }

  ce->down = e->down;
#endif

  e->left->right = e->right;
  e->right->left = e->left;

  e->left = m->next_free;
  m->next_free = e;
}

#ifdef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
mod2entry* mod2sparse_last_in_col(mod2sparse* m, int i) {
  mod2entry* e;
  e = mod2sparse_first_in_col(m, i);

  while (!mod2sparse_at_end(e->down)) {
    e = e->down;
  }

  return e;
}
#endif
