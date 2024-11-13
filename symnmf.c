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


//recieve mat (nXm)
//return transposed mat (mXn)
double **mat_transpose(double **mat, int n, int m){
    int i, j;
    double **trans_mat = calloc(m * n, sizeof(double));
    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            trans_mat[j][i] = mat[i][j];
        }
    }
    return trans_mat;
}

//updates h by formula
double **update_h(double **prev_h, double **w, double beta, int n, int k){
    int i, j;
    double **new_h = calloc(n * k, sizeof(double));
    double **numerator = mult_mat(w, prev_h, n, n, k);
    double **h_on_ht = mult_mat(prev_h, mat_transpose(prev_h, n, k), n , k, n);; //temp to calc the denominator
    double **denominator = mult_mat(h_on_ht, prev_h, n, n, k);

    
    for (i = 0; i < n; i++){
        for (j = 0; j < k; j++){
            new_h[i][j] = (1- beta + beta * (numerator[i][j] / denominator[i][j]));
        }
    }
    return new_h;
}

int main(int argc, char **argv)
{
    return 0;
}