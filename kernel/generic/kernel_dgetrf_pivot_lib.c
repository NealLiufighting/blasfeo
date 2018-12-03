/**************************************************************************************************
*                                                                                                 *
* This file is part of BLASFEO.                                                                   *
*                                                                                                 *
* BLASFEO -- BLAS For Embedded Optimization.                                                      *
* Copyright (C) 2016-2018 by Gianluca Frison.                                                     *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* This program is free software: you can redistribute it and/or modify                            *
* it under the terms of the GNU General Public License as published by                            *
* the Free Software Foundation, either version 3 of the License, or                               *
* (at your option) any later version                                                              *.
*                                                                                                 *
* This program is distributed in the hope that it will be useful,                                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                   *
* GNU General Public License for more details.                                                    *
*                                                                                                 *
* You should have received a copy of the GNU General Public License                               *
* along with this program.  If not, see <https://www.gnu.org/licenses/>.                          *
*                                                                                                 *
* The authors designate this particular file as subject to the "Classpath" exception              *
* as provided by the authors in the LICENSE file that accompained this code.                      *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#include <math.h>
#include <stdio.h>

#include "../../include/blasfeo_common.h"
#include "../../include/blasfeo_d_aux.h"



// C numbering, starting from 0
void didamax_lib(int n, double *pA, int *p_idamax, double *p_amax)
	{

	int idamax, ii;
	double tmp, amax;
		
	p_idamax[0] = -1;
	if(n<1)
		return;

	amax = -1.0;
	ii = 0;
	for( ; ii<n-3; ii+=4)
		{
		tmp = fabs(pA[0]);
		if(tmp>amax)
			{
			idamax = ii+0;
			amax = tmp;
			}
		tmp = fabs(pA[1]);
		if(tmp>amax)
			{
			idamax = ii+1;
			amax = tmp;
			}
		tmp = fabs(pA[2]);
		if(tmp>amax)
			{
			idamax = ii+2;
			amax = tmp;
			}
		tmp = fabs(pA[3]);
		if(tmp>amax)
			{
			idamax = ii+3;
			amax = tmp;
			}
		pA += 4;
		}
	for( ; ii<n; ii++)
		{
		tmp = fabs(pA[0]);
		if(tmp>amax)
			{
			idamax = ii+0;
			amax = tmp;
			}
		pA += 1;
		}
	
	p_amax[0] = amax;
	p_idamax[0] = idamax;

	return;

	}



// C numering (starting from zero) in the ipiv
// it process m>=4 rows and 4 cols
void kernel_dgetrf_pivot_4_lib(int m, double *pA, int lda, double *inv_diag_A, int* ipiv)
	{

	// assume m>=4
	int ma = m-4;

	double
		tmp0, tmp1, tmp2, tmp3,
		u_00, u_01, u_02, u_03,
		      u_11, u_12, u_13,
		            u_22, u_23,
		                  u_33;
	
	double
		*pB;
	
	int 
		k, idamax;
	
	int
		ia0, ia1, ia2;

	// first column
	didamax_lib(m-0, &pA[0+lda*0], &idamax, &tmp0);
	ipiv[0] = idamax;
	if(tmp0!=0.0)
		{
		if(ipiv[0]!=0)
			{
			ia0 = 4;
			ia1 = 0;
			ia2 = ia1+1;
			blasfeo_dlaswp(&ia0, pA, &lda, &ia1, &ia1, ipiv, &ia2);
			}

		tmp0 = 1.0 / pA[0+lda*0];
		inv_diag_A[0] = tmp0;
		pA[1+lda*0] *= tmp0;
		pA[2+lda*0] *= tmp0;
		pA[3+lda*0] *= tmp0;
		pB = pA + 4;
		for(k=0; k<ma-3; k+=4)
			{
			pB[0+lda*0] *= tmp0;
			pB[1+lda*0] *= tmp0;
			pB[2+lda*0] *= tmp0;
			pB[3+lda*0] *= tmp0;
			pB += 4;
			}
		for( ; k<ma; k++)
			{
			pB[0+lda*0] *= tmp0;
			pB += 1;
			}
		}
	else
		{
		inv_diag_A[0] = 0.0;
		}

	// second column
	u_01  = pA[0+lda*1];
	tmp1  = pA[1+lda*1];
	tmp2  = pA[2+lda*1];
	tmp3  = pA[3+lda*1];
	tmp1 -= pA[1+lda*0] * u_01;
	tmp2 -= pA[2+lda*0] * u_01;
	tmp3 -= pA[3+lda*0] * u_01;
	pA[1+lda*1] = tmp1;
	pA[2+lda*1] = tmp2;
	pA[3+lda*1] = tmp3;
	pB = pA + 4;
	for(k=0; k<ma-3; k+=4)
		{
		tmp0  = pB[0+lda*1];
		tmp1  = pB[1+lda*1];
		tmp2  = pB[2+lda*1];
		tmp3  = pB[3+lda*1];
		tmp0 -= pB[0+lda*0] * u_01;
		tmp1 -= pB[1+lda*0] * u_01;
		tmp2 -= pB[2+lda*0] * u_01;
		tmp3 -= pB[3+lda*0] * u_01;
		pB[0+lda*1] = tmp0;
		pB[1+lda*1] = tmp1;
		pB[2+lda*1] = tmp2;
		pB[3+lda*1] = tmp3;
		pB += 4;
		}
	for( ; k<ma; k++)
		{
		tmp0 = pB[0+lda*1];
		tmp0 -= pB[0+lda*0] * u_01;
		pB[0+lda*1] = tmp0;
		pB += 1;
		}

	didamax_lib(m-1, &pA[1+lda*1], &idamax, &tmp1);
	ipiv[1] = idamax+1;
	if(tmp1!=0)
		{
		if(ipiv[1]!=1)
			{
			ia0 = 4;
			ia1 = 1;
			ia2 = ia1+1;
			blasfeo_dlaswp(&ia0, pA, &lda, &ia1, &ia1, ipiv, &ia2);
			}

		tmp1 = 1.0 / pA[1+lda*1];
		inv_diag_A[1] = tmp1;
		pA[2+lda*1] *= tmp1;
		pA[3+lda*1] *= tmp1;
		pB = pA + 4;
		for(k=0; k<ma-3; k+=4)
			{
			pB[0+lda*1] *= tmp1;
			pB[1+lda*1] *= tmp1;
			pB[2+lda*1] *= tmp1;
			pB[3+lda*1] *= tmp1;
			pB += 4;
			}
		for( ; k<ma; k++)
			{
			pB[0+lda*1] *= tmp1;
			pB += 1;
			}
		}
	else
		{
		inv_diag_A[1] = 0.0;
		}

	// third column
	u_02  = pA[0+lda*2];
	u_12  = pA[1+lda*2];
	u_12 -= pA[1+lda*0] * u_02;
	pA[1+lda*2] = u_12;
	tmp2  = pA[2+lda*2];
	tmp3  = pA[3+lda*2];
	tmp2 -= pA[2+lda*0] * u_02;
	tmp3 -= pA[3+lda*0] * u_02;
	tmp2 -= pA[2+lda*1] * u_12;
	tmp3 -= pA[3+lda*1] * u_12;
	pA[2+lda*2] = tmp2;
	pA[3+lda*2] = tmp3;
	pB = pA + 4;
	for(k=0; k<ma-3; k+=4)
		{
		tmp0  = pB[0+lda*2];
		tmp1  = pB[1+lda*2];
		tmp2  = pB[2+lda*2];
		tmp3  = pB[3+lda*2];
		tmp0 -= pB[0+lda*0] * u_02;
		tmp1 -= pB[1+lda*0] * u_02;
		tmp2 -= pB[2+lda*0] * u_02;
		tmp3 -= pB[3+lda*0] * u_02;
		tmp0 -= pB[0+lda*1] * u_12;
		tmp1 -= pB[1+lda*1] * u_12;
		tmp2 -= pB[2+lda*1] * u_12;
		tmp3 -= pB[3+lda*1] * u_12;
		pB[0+lda*2] = tmp0;
		pB[1+lda*2] = tmp1;
		pB[2+lda*2] = tmp2;
		pB[3+lda*2] = tmp3;
		pB += 4;
		}
	for( ; k<ma; k++)
		{
		tmp0  = pB[0+lda*2];
		tmp0 -= pB[0+lda*0] * u_02;
		tmp0 -= pB[0+lda*1] * u_12;
		pB[0+lda*2] = tmp0;
		pB += 1;
		}

	didamax_lib(m-2, &pA[2+lda*2], &idamax, &tmp2);
	ipiv[2] = idamax+2;
	if(tmp2!=0)
		{
		if(ipiv[2]!=2)
			{
			ia0 = 4;
			ia1 = 2;
			ia2 = ia1+1;
			blasfeo_dlaswp(&ia0, pA, &lda, &ia1, &ia1, ipiv, &ia2);
			}

		tmp2 = 1.0 / pA[2+lda*2];
		inv_diag_A[2] = tmp2;
		pA[3+lda*2] *= tmp2;
		pB = pA + 4;
		for(k=0; k<ma-3; k+=4)
			{
			pB[0+lda*2] *= tmp2;
			pB[1+lda*2] *= tmp2;
			pB[2+lda*2] *= tmp2;
			pB[3+lda*2] *= tmp2;
			pB += 4;
			}
		for( ; k<ma; k++)
			{
			pB[0+lda*2] *= tmp2;
			pB += 1;
			}
		}
	else
		{
		inv_diag_A[2] = 0.0;
		}

	// fourth column
	u_03  = pA[0+lda*3];
	u_13  = pA[1+lda*3];
	u_13 -= pA[1+lda*0] * u_03;
	pA[1+lda*3] = u_13;
	u_23  = pA[2+lda*3];
	u_23 -= pA[2+lda*0] * u_03;
	u_23 -= pA[2+lda*1] * u_13;
	pA[2+lda*3] = u_23;
	tmp3  = pA[3+lda*3];
	tmp3 -= pA[3+lda*0] * u_03;
	tmp3 -= pA[3+lda*1] * u_13;
	tmp3 -= pA[3+lda*2] * u_23;
	pA[3+lda*3] = tmp3;
	pB = pA + 4;
	for(k=0; k<ma-3; k+=4)
		{
		tmp0  = pB[0+lda*3];
		tmp1  = pB[1+lda*3];
		tmp2  = pB[2+lda*3];
		tmp3  = pB[3+lda*3];
		tmp0 -= pB[0+lda*0] * u_03;
		tmp1 -= pB[1+lda*0] * u_03;
		tmp2 -= pB[2+lda*0] * u_03;
		tmp3 -= pB[3+lda*0] * u_03;
		tmp0 -= pB[0+lda*1] * u_13;
		tmp1 -= pB[1+lda*1] * u_13;
		tmp2 -= pB[2+lda*1] * u_13;
		tmp3 -= pB[3+lda*1] * u_13;
		tmp0 -= pB[0+lda*2] * u_23;
		tmp1 -= pB[1+lda*2] * u_23;
		tmp2 -= pB[2+lda*2] * u_23;
		tmp3 -= pB[3+lda*2] * u_23;
		pB[0+lda*3] = tmp0;
		pB[1+lda*3] = tmp1;
		pB[2+lda*3] = tmp2;
		pB[3+lda*3] = tmp3;
		pB += 4;
		}
	for( ; k<ma; k++)
		{
		tmp0  = pB[0+lda*3];
		tmp0 -= pB[0+lda*0] * u_03;
		tmp0 -= pB[0+lda*1] * u_13;
		tmp0 -= pB[0+lda*2] * u_23;
		pB[0+lda*3] = tmp0;
		pB += 1;
		}

	didamax_lib(m-3, &pA[3+lda*3], &idamax, &tmp3);
	ipiv[3] = idamax+3;
	if(tmp3!=0)
		{
		if(ipiv[3]!=3)
			{
			ia0 = 4;
			ia1 = 3;
			ia2 = ia1+1;
			blasfeo_dlaswp(&ia0, pA, &lda, &ia1, &ia1, ipiv, &ia2);
			}

		tmp3 = 1.0 / pA[3+lda*3];
		inv_diag_A[3] = tmp3;
		pB = pA + 4;
		for(k=0; k<ma-3; k+=4)
			{
			pB[0+lda*3] *= tmp3;
			pB[1+lda*3] *= tmp3;
			pB[2+lda*3] *= tmp3;
			pB[3+lda*3] *= tmp3;
			pB += 4;
			}
		for( ; k<ma; k++)
			{
			pB[0+lda*3] *= tmp3;
			pB += 1;
			}
		}
	else
		{
		inv_diag_A[3] = 0.0;
		}
	
	return;

	}



