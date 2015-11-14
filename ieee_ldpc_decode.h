//
//  decode.h
//  MyLdpc
//
//  Created by Wing on 4/29/15.
//  Copyright (c) 2015 Wing. All rights reserved.
//

#ifndef __MyLdpc__decode__
#define __MyLdpc__decode__

#include <stdio.h>
#include"matrix.h"
#define ERR -1
int decode(matrix *matrix_n,matrix* matrix_h);

int cl_decode(matrix *matrix_n,matrix* matrix_h);

int decode2(matrix *matrix_n,matrix* matrix_h);

int check_result(matrix *matrix_n,matrix* matrix_h);
#endif /* defined(__MyLdpc__decode__) */
