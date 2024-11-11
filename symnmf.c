#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Make sure all numbers use double

double **ddg(double **data, int n)
{
    // Computes diagonal degree matrix
    double **ret = calloc(n * n, sizeof(double));
    return ret;
}

double **sym(double **data, int n)
{
    // Computes similarity matrix
    double **ret = calloc(n * n, sizeof(double));
    return ret;
}

double **norm(double **data, int n)
{
    // Computes normalized similarity matrix
    double **ret = calloc(n * n, sizeof(double));
    return ret;
}

double **mult_mat(double **matA, double **matK, int n, int k, int m)
{
    // Matrix A: k*n, Matrix B: n*m, Matrix A*B: k*m
    double **ret = calloc(k * m, sizeof(double));
    return ret;
}

double **symnmf(double **H_i, double **W, int n, int k)
{
    return H_i;
}

void print_mat(double **mat, int n, int m)
{
    // Prints a matrix of size n*m
    return;
}

int main(int argc, char **argv)
{
    return 0;
}