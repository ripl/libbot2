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

#include "tools.h"

#include <stdio.h>
#include <stdlib.h>

// ALLOCATE SPACE AND CHECK FOR ERROR.  Calls 'calloc' to allocate space,
// and then displays an error message and exits if the space couldn't be
// found.

void* chk_alloc(unsigned n,  // Number of elements
                unsigned size  // Size of each element
) {
  void* p;

  p = calloc(n, size);

  if (p == 0) {
    fprintf(stderr, "Ran out of memory (while trying to allocate %d bytes)\n",
            n * size);
    exit(1);
  }

  return p;
}
