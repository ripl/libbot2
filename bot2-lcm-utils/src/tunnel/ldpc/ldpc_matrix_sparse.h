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

// ldpc_matrix.h - Interface to module for handling sparse mod2 matrices.

// This module implements operations on sparse matrices of mod2 elements
// (bits, with addition and multiplication being done modulo 2).
//
// All procedures in this module display an error message on standard
// error and terminate the program if passed an invalid argument (indicative
// of a programming error), or if memory cannot be allocated.  Errors from
// invalid contents of a file result in an error code being returned to the
// caller, with no message being printed by this module.

// DATA STRUCTURES USED TO STORE A SPARSE MATRIX.  Non-zero entries (ie, 1s)
// are represented by nodes that are doubly-linked both by row and by column,
// with the headers for these lists being kept in arrays.  Nodes are allocated
// in blocks to reduce time and space overhead.  Freed nodes are kept for
// reuse in the same matrix, rather than being freed for other uses, except
// that they are all freed when the matrix is cleared to all zeros by the
// mod2sparse_clear procedure, or copied into by mod2sparse_copy.
//
// Direct access to these structures should be avoided except in low-level
// routines.  Use the macros and procedures defined below instead.

#ifndef BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_MATRIX_SPARSE_H_
#define BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_MATRIX_SPARSE_H_

#include <stdio.h>

/**
 * Structure representing a non-zero entry, or the header for a row or column.
 */
typedef struct mod2entry {
  /**
   * Row and column indexes of this entry, starting at 0, and
   * with -1 for a row or column header
   */
#ifdef SPARSE_MATRIX_OPT_SMALL_INDEXES
  // memory opt., see ldpc_profile.h
  short row;
  short col;
#else
  int row;
  int col;
#endif

  /**
   * Pointers to entries adjacent in row and column, or to headers.
   * Free entries are linked by 'left'.
   */
  struct mod2entry *left, *right, *down;
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
  // memory opt., see ldpc_profile.h
  struct mod2entry* up;
#endif
} mod2entry;

// Number of entries to block together for memory allocation
#define Mod2sparse_block 10

/**
 * Block of entries allocated all at once.
 */
typedef struct mod2block {
  /** Next block that has been allocated. **/
  struct mod2block* next;

  /** Entries in this block. **/
  mod2entry entry[Mod2sparse_block];
} mod2block;

/**
 * Representation of a sparse matrix.
 */
typedef struct mod2sparse {
  int n_rows;  // Number of rows in the matrix
  int n_cols;  // Number of columns in the matrix

  mod2entry* rows;  // Pointer to array of row headers
  mod2entry* cols;  // Pointer to array of column headers

  mod2block* blocks;  // Blocks that have been allocated
  mod2entry* next_free;  // Next free entry
} mod2sparse;

// MACROS TO GET AT ELEMENTS OF A SPARSE MATRIX.  The 'first', 'last', 'next',
// and 'prev' macros traverse the elements in a row or column.  Moving past
// the first/last element gets one to a header element, which can be identified
// using the 'at_end' macro.  Macros also exist for finding out the row
// and column of an entry, and for finding out the dimensions of a matrix.

// Find the first
#define mod2sparse_first_in_row(m, i) ((m)->rows[i].right)
// or last entry in
#define mod2sparse_first_in_col(m, j) ((m)->cols[j].down)
// a row or column
#define mod2sparse_last_in_row(m, i) ((m)->rows[i].left)
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
#define mod2sparse_last_in_col(m, j) ((m)->cols[j].up)
#endif

// Move from one entry to
#define mod2sparse_next_in_row(e) ((e)->right)
// another in any of the three
#define mod2sparse_next_in_col(e) ((e)->down)
// possible directions
#define mod2sparse_prev_in_row(e) ((e)->left)
#ifndef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
#define mod2sparse_prev_in_col(e) ((e)->up)
#endif

// See if we've reached the end
#define mod2sparse_at_end(e) ((e)->row < 0)

// Find out the row or column index
#define mod2sparse_row(e) ((e)->row)
// of an entry (indexes start at 0)
#define mod2sparse_col(e) ((e)->col)

// Get the number of rows or columns
#define mod2sparse_rows(m) ((m)->n_rows)
// in a matrix
#define mod2sparse_cols(m) ((m)->n_cols)

// PROCEDURES TO MANIPULATE SPARSE MATRICES.

mod2sparse* mod2sparse_allocate(int, int);
void mod2sparse_free(mod2sparse*);

void mod2sparse_clear(mod2sparse*);

void mod2sparse_print(FILE*, mod2sparse*);

mod2entry* mod2sparse_find(mod2sparse*, int, int);
mod2entry* mod2sparse_insert(mod2sparse*, int, int);
void mod2sparse_delete(mod2sparse*, mod2entry*);

#ifdef SPARSE_MATRIX_OPT_FOR_LDPC_STAIRCASE
mod2entry* mod2sparse_last_in_col(mod2sparse* m, int i);
#endif

#endif  // BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_MATRIX_SPARSE_H_
