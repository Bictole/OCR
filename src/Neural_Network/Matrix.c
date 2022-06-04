#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <math.h>

#include "matrixType.h"

matrix *build(size_t row, size_t col)
{
    matrix *mat = malloc(sizeof(matrix));
    mat->rows = row;
    mat->cols = col;
    mat->data = calloc(row * col, sizeof(double));
    return mat;
}



void mat_mod_val(matrix* mat, int r, int c, double val)
{
    mat->data[r * mat->cols + c] = val;
}

void mat_mod_add_val(matrix* mat, int r, int c, double val)
{
    mat->data[r * mat->cols + c] += val;
}

matrix *mat_transpose(matrix* m)
{
    matrix *new = build(m->cols, m->rows);

    for(int n = 0; n<m->cols*m->rows; n++)
    {
        int i = n/m->rows;
        int j = n%m->rows;
        new->data[n] = m->data[m->cols*j + i];
    }
    return new;
}

void mat_random_fill(matrix* mat)
{
    srand(time(NULL));

    for(int x = 0; x < mat->rows; x++)
    {
        for(int y = 0; y < mat->cols; y++)
        {
            mat->data[x*mat->cols + y] = (double) (rand() % RAND_MAX)/RAND_MAX - (double) (rand() % RAND_MAX)/RAND_MAX;
        }
    }
}

void mat_fill(matrix* mat, double val)
{
    for(int x = 0; x < mat->rows; x++)
    {
        for(int y = 0; y < mat->cols; y++)
        {
            mat->data[x*mat->cols+y] = val;
        }
    }
}

matrix *mat_mult(matrix* m, double n)
{
    for (int c = 0; c<m->cols; c++)
    {
        for (int r = 0; r<m->rows; r++)
        {
            m->data[r*m->cols + c] *= n;
        }
    }
    return m;
}

matrix* mat_memb_mult(matrix* src1, matrix* src2)//Create new matrix instead
{
    if (src1->cols != src2->cols && src1->rows != src2->rows)
        errx("1", "Wrong multiplication dimensions");
    int w,h;
    h = src1->rows > src2->rows ? src1->rows : src2->rows;
    w = src1->cols > src2->cols ? src1->cols : src2->cols;

    matrix *mat = build(h, w);

    for(int i = 0; i < mat->cols; i++)
    {
        for (int j = 0; j < mat->rows; j++)
        mat->data[j*mat->cols + i] =
        src1->data[(j%src1->rows)*src1->cols + (i%src1->cols)] *
        src2->data[(j%src1->rows)*src2->cols + (i%src2->cols)];
    }

    return mat;
}

matrix* mat_add(matrix* m1, matrix* m2)
{
    //matrix *nam = build(m1->rows, m1->cols);
    if (m1->cols != m2->cols || m1->rows != m2->rows)
        errx("69", "Wrong sum dimensions");
    for (size_t r = 0; r < m1->rows*m2->cols; r++)
    {
        m1->data[r] += m2->data[r];
    }
    return m1;
}

matrix* mat_sub(matrix* m1, matrix* m2)
{
    if (m1->cols != m2->cols || m1->rows != m2->rows)
        errx("69", "Wrong dimensions");
    matrix *nam = build(m1->rows, m1->cols);
    for (size_t r = 0; r < m1->rows*m2->cols; r++)
    {
        nam->data[r] = m1->data[r] - m2->data[r];
    }
    return nam;
}

matrix* mat_dot(matrix* m1, matrix* m2)
{
    if (m1->cols != m2->rows)
        errx(2, "Wrong dimensions!");
    if(m1->rows == 1 && m1->cols == 1)
    {
        return mat_mult(m2, m1->data[0]);
    }

    if (m2->rows == 1 && m2->cols == 1)
    {
        return mat_mult(m1, m2->data[0]);
    }
    matrix* nw = build(m1->rows, m2->cols);

    for (int crow = 0; crow < nw->rows; crow++)
    {
        for (int ccol = 0; ccol < nw->cols; ccol++)
        {
            double summa = 0;
            for (int k = 0; k<m1->cols; k++)
            {
                 summa+=
                m1->data[crow * m1->cols + k] *
                m2->data[k * m2->cols + ccol];
            }
            nw->data[crow * nw->cols + ccol] = summa;
        }
    }
    return nw;
}

void mat_free(matrix* m)
{
    free(m->data);
    free(m);
}

void mat_apply(matrix* m, void (*f)(double))
{
    for (int i = 0; i<m->cols*m->rows; i++)
    {
        (*f) (m->data[i]);
    }
}

void print_matrix(matrix* mat)
{
	for (size_t row = 0; row < mat->rows; row++)
	{
		for (size_t col = 0; col < mat->cols; col++)
			printf("%2g ", mat->data[row * mat->cols + col]);
		putchar('\n');
	}
}


matrix* SoftMax(matrix* mat)//Gives percentile distribution
{
    double *list = mat->data;
    size_t input_len = (size_t)(mat->cols*mat->rows);
    double m = -INFINITY;
    for (size_t i = 0; i < input_len; i++)
    {
        if (list[i] > m)
        {
            m = list[i];
        }
    }

    double sum = 0.0;
    for (size_t i = 0; i < input_len; i++)
    {
        sum += expf(list[i] - m);
    }

    double offset = m + logf(sum);
    for (size_t i = 0; i < input_len; i++)
    {
        list[i] = expf(list[i] - offset);
    }
    return mat;
}

double sigmoido(double z)
{
    return 1/(1+expf(-z));
}

matrix *sigmoid(matrix *mat)
{
    matrix *nmat = build(mat->rows, mat->cols);
    for (int i = 0; i < mat->rows*mat->cols; i++)
    {
        nmat->data[i] = sigmoido(mat->data[i]);
    }
    return nmat;
}

matrix *sigmoidPrime(matrix *mat)
{
    matrix *sigmat = build(mat->rows, mat->cols);
    for (int i = 0; i < mat->rows*mat->cols; i++)
    {
        sigmat->data[i] = mat->data[i]/(1-mat->data[i]);
    }
    return sigmat;
}
