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

#include "ldpc_create_pchk.h"

#include <stdio.h>
#include <stdlib.h>

#include "ldpc_rand.h"
#include "tools.h"

mod2sparse* CreatePchkMatrix (  int nbRows, int nbCols, make_method makeMethod, int leftDegree, int seed, bool no4cycle, SessionType type, int verbosity )
{
	mod2entry *e;
	int added, uneven;
	int i, j, k, t;
	int *u;
	mod2sparse *pchkMatrix = NULL;
	int skipCols = 0;		// avoid warning
	int nbDataCols = 0;		// avoid warning

	if (type != TypeLDGM && type != TypeSTAIRS && type != TypeTRIANGLE) {
		fprintf(stderr, "unsupported code type (%d)\n", type);
		return NULL;
	}
	skipCols = nbRows;
	nbDataCols = nbCols-skipCols;

	// Check for some problems.
	if (leftDegree>nbRows)
	{
		fprintf(stderr, "ERROR, Number of checks per bit (%d) is greater than total checks (%d)\n", leftDegree, nbRows);
		return NULL;
	}
	if (no4cycle) {
		fprintf(stderr, "ERROR: no4cycle mode is no longer supported!\n");
		exit(-1);
	}

	ldpc_srand(seed);
	pchkMatrix = mod2sparse_allocate(nbRows, nbCols);

	/* Create the initial version of the parity check matrix. */
	switch (makeMethod)
	{
	case Evencol:
		for(j=skipCols; j<nbCols; j++)
		{
			for(k=0; k<leftDegree; k++)
			{
				do
				{
					i = ldpc_rand(nbRows);
				}
				while (mod2sparse_find(pchkMatrix,i,j));
				mod2sparse_insert(pchkMatrix,i,j);
			}
		}
		break;

	case Evenboth:
		u = (int*)chk_alloc (leftDegree*nbDataCols, sizeof *u);

		/* initialize a list of possible choices to guarantee a homogeneous "1" distribution */
		for(k = leftDegree*nbDataCols-1; k>=0; k--)
		{
			u[k] = k%nbRows;
		}
		uneven = 0;
		t = 0;	/* left limit within the list of possible choices, u[] */
		for(j = skipCols; j<nbCols; j++)	/* for each source symbol column */
		{
			for(k = 0; k<leftDegree; k++)	/* add left_degree "1s" */
			{
				/* check that valid available choices remain */
				for(i = t; i<leftDegree*nbDataCols && mod2sparse_find(pchkMatrix,u[i],j); i++) ;

				if(i < leftDegree*nbDataCols)
				{
					/* choose one index within the list of possible choices */
					do {
						i = t + ldpc_rand(leftDegree*nbDataCols-t);
					} while (mod2sparse_find(pchkMatrix,u[i],j));
					mod2sparse_insert(pchkMatrix,u[i],j);
					/* replace with u[t] which has never been chosen */
					u[i] = u[t];
					t++;
				}
				else
				{
					/* no choice left, choose one randomly */
					uneven += 1;
					do {
						i = ldpc_rand(nbRows);
					} while (mod2sparse_find(pchkMatrix,i,j));
					mod2sparse_insert(pchkMatrix,i,j);
				}
			}
		}

		if(uneven > 0 && verbosity >= 1)
		{
			fprintf(stderr,"Had to place %d checks in rows unevenly\n",uneven);
		}
		free(u);	/* VR: added */
		break;

	default: abort();
	}

	/* Add extra bits to avoid rows with less than two checks. */
	added = 0;
	for(i = 0; i<nbRows; i++)
	{
		e = mod2sparse_first_in_row(pchkMatrix,i);
		if(mod2sparse_at_end(e))
		{
			j = (ldpc_rand(nbDataCols))+skipCols;
			e = mod2sparse_insert(pchkMatrix,i,j);
			added ++;
		}
		e = mod2sparse_first_in_row(pchkMatrix,i);
		if(mod2sparse_at_end(mod2sparse_next_in_row(e)) && nbDataCols>1)
		{
			do
			{
				j = (ldpc_rand(nbDataCols))+skipCols;
			} while (j==mod2sparse_col(e));
			mod2sparse_insert(pchkMatrix,i,j);
			added ++;
		}
	}

	if(added > 0 && verbosity >= 1)
	{
		fprintf(stderr, "Added %d extra bit-checks to make row counts at least two\n", added);
	}

	/* Add extra bits to try to avoid problems with even column counts. */
	if(leftDegree%2==0 && leftDegree<nbRows && nbDataCols>1 && added<2)
	{
		int a;
		for(a = 0; added+a<2; a++)
		{
			do
			{
				i = ldpc_rand(nbRows);
				j = (ldpc_rand(nbDataCols))+skipCols;
			} while (mod2sparse_find(pchkMatrix,i,j));
			mod2sparse_insert(pchkMatrix,i,j);
		}
		if (verbosity >= 1) {
			fprintf(stderr, "Added %d extra bit-checks to try to avoid problems from even column counts\n", a);
		}
	}

	switch (type) {
	case TypeLDGM:
		for (i = 0; i < nbRows; i++) {
			/* identity */
			mod2sparse_insert(pchkMatrix, i, i);
		}
		break;

	case TypeSTAIRS:
		mod2sparse_insert(pchkMatrix, 0, 0);	/* 1st row */
		for (i = 1; i < nbRows; i++) {		/* for all other rows */
			/* identity */
			mod2sparse_insert(pchkMatrix, i, i);
			/* staircase */
			mod2sparse_insert(pchkMatrix, i, i-1);
		}
		break;

	case TypeTRIANGLE:
		mod2sparse_insert(pchkMatrix, 0, 0);	/* 1st row */
		for (i = 1; i < nbRows; i++) {		/* for all other rows */
			/* identity */
			mod2sparse_insert(pchkMatrix, i, i);
			/* staircase */
			mod2sparse_insert(pchkMatrix, i, i-1);
			/* triangle */
			j = i-1;
			for (int l = 0; l < j; l++) { /* limit the # of "1s" added */
				j = ldpc_rand(j);
				mod2sparse_insert(pchkMatrix, i, j);
			}
		}
		break;
	}

	return pchkMatrix;
}

