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

#include <stdlib.h>
#include <stdio.h>

#include "../include/blasfeo_common.h"
#include "../include/blasfeo_d_aux.h"



#define REAL double

#define STRMAT blasfeo_dmat_ref
#define STRVEC blasfeo_dvec_ref

#define GEMV_N_LIBSTR blasfeo_dgemv_n_ref
#define GEMV_NT_LIBSTR blasfeo_dgemv_nt_ref
#define GEMV_T_LIBSTR blasfeo_dgemv_t_ref
#define SYMV_L_LIBSTR blasfeo_dsymv_l_ref
#define TRMV_LNN_LIBSTR blasfeo_dtrmv_lnn_ref
#define TRMV_LTN_LIBSTR blasfeo_dtrmv_ltn_ref
#define TRMV_UNN_LIBSTR blasfeo_dtrmv_unn_ref
#define TRMV_UTN_LIBSTR blasfeo_dtrmv_utn_ref
#define TRSV_LNN_LIBSTR blasfeo_dtrsv_lnn_ref
#define TRSV_LNN_MN_LIBSTR blasfeo_dtrsv_lnn_mn_ref
#define TRSV_LNU_LIBSTR blasfeo_dtrsv_lnu_ref
#define TRSV_LTN_LIBSTR blasfeo_dtrsv_ltn
#define TRSV_LTN_MN_LIBSTR blasfeo_dtrsv_ltn_mn_ref
#define TRSV_LTU_LIBSTR blasfeo_dtrsv_ltu_ref
#define TRSV_UNN_LIBSTR blasfeo_dtrsv_unn_ref
#define TRSV_UTN_LIBSTR blasfeo_dtrsv_utn_ref


#include "x_blas2_lib.c"
