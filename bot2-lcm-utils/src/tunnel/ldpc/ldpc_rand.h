/*
 *  LDPC/LDGM FEC Library.
 *  (c) Copyright 2002-2006 INRIA - All rights reserved
 *  Main authors: Christoph Neumann (christoph.neumann@inrialpes.fr)
 *                Vincent Roca      (vincent.roca@inrialpes.fr)
 *                Julien Laboure    (julien.laboure@inrialpes.fr)
 *
 *  This copyright notice must be retained and prominently displayed,
 *  along with a note saying that the original programs are available from
 *  Vincent Roca's web page, and note is made of any changes made to these
 *  programs.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_RAND_H_
#define BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_RAND_H_

/**
 * Initialize the PRNG with a seed between 1 and 0x7FFFFFFE
 * (2^^31-2) inclusive.
 */
void		ldpc_srand (unsigned long s);

/**
 * Returns a random integer between 0 and maxv-1 inclusive.
 * Derived from rand31pmc, Robin Whittle, Sept 20th 2005.
 * http://www.firstpr.com.au/dsp/rand31/
 *	16807		multiplier constant (7^^5)
 *	0x7FFFFFFF	modulo constant (2^^31-1)
 * The inner PRNG produces a value between 1 and 0x7FFFFFFE
 * (2^^31-2) inclusive.
 * This value is then scaled between 0 and maxv-1 inclusive.
 */
unsigned long	ldpc_rand (unsigned long	maxv);

#endif  // BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_RAND_H_
