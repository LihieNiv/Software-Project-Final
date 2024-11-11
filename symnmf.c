#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Make sure all numbers use double

typedef struct cord
{
    double value;
    cord *next;
} cord;

typedef struct vector
{
    vector *next;
    cord *cords;
} vector;

// Computes diagonal degree matrix
double **ddg(vector data, int n)
{
    double **ret = calloc(n * n, sizeof(double));
    return ret;
}
// Computes similarity between two vectors
double sym_func(cord x, cord y)
{
    return 0;
}

// Computes similarity matrix
double **sym(vector data, int n)
{
    double **ret = calloc(n * n, sizeof(double));
    return ret;
}

// Computes normalized similarity matrix
double **norm(vector data, int n)
{
    double **ret = calloc(n * n, sizeof(double));
    return ret;
}

// Matrix A: k*n, Matrix B: n*m, Matrix A*B: k*m
double **mult_mat(double **matA, double **matB, int n, int k, int m)
{
    double **ret = calloc(k * m, sizeof(double));
    int i, j, p, sum;
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < m; j++)
        {
            sum = 0;
            for (p = 0; p < n; p++)
            {
                sum += matA[i][p] * matB[p][j];
            }
            ret[i][j] = sum;
        }
    }
    return ret;
}

double **symnmf(double **H_i, double **W, int n, int k)
{
    return H_i;
}

// Prints a matrix of size n*m
void print_mat(double **mat, int n, int m)
{

    return;
}

int main(int argc, char **argv)
{
    return 0;
}