#ifndef MATRIX_H
#define MATRIX_H

//The Definers

#include "matrixType.h"

matrix* build(int row, int col);


//The Manipulators


matrix *mat_sub(matrix* m1, matrix* m2);

matrix *mat_dot(matrix* m1, matrix* m2);

void mat_mod_val(matrix* mat, int r, int c, double val);

matrix* mat_transpose(matrix* m);

matrix* mat_mult(matrix* m, double n);

matrix* mat_memb_mult(matrix* src1, matrix* src2);

matrix* mat_add(matrix* m1, matrix* m2);

void mat_random_fill(matrix* mat);

void mat_fill(matrix* mat, double val);

matrix* SoftMax(matrix* m);

matrix *sigmoid(matrix *mat);

matrix *sigmoidPrime(matrix *mat);

void print_matrix(matrix* m);

void mat_free(matrix* m);

#endif
