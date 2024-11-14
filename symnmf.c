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

//calc square euclidean distsance
double euc_dst(struct vector *x, struct vector *y){
    double sum = 0.0;
    struct cord *curr_cord_x = x->cords;
    struct cord *curr_cord_y = y->cords;
        while(curr_cord_x != NULL){
            sum += pow(curr_cord_x->value - curr_cord_y->value, 2);
            curr_cord_x = curr_cord_x->next;
            curr_cord_y = curr_cord_y->next;
    }
    return sum;
}

// Computes similarity between two vectors
double sym_func(cord x, cord y){
    return 0;
}

// Computes similarity matrix from the data which is an arr of vectors.
double **sym(vector *data[], int n){
    int i, j;
    double **result_mat = calloc(n * n, sizeof(double));

    for (i = 0; i < n; i++){
        result_mat[i][i] = 0;
        for (j = i+1; j < n; j++){
            result_mat[i][j] = result_mat[j][i] = exp(-euc_dst(data[i], data[j]) / 2);
        }
    }
    return result_mat;
}


// Computes diagonal degree matrix size nXn of mat size n X m
double **ddg(double ** mat, int n, int m){
    int i, j, sum_col;
    double **result_mat = calloc(n * n, sizeof(double));

    for(i = 0; i < n; i++){
        sum_col = 0;
        for (j = 0; j < m; j++){
            sum_col += mat[i][j];
            result_mat[i][j] = 0;
        }
        result_mat[i][i] = sum_col; //change only on the diag
    }
    
    return result_mat;
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
    for (i = 0; i < k; i++){
        for (j = 0; j < m; j++){
            sum = 0;
            for (p = 0; p < n; p++){
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


//calc A - B, size n X m each.
double **diff_mat(double **a, double **b, int n, int m){
    int i, j;
    double **result_mat = calloc(n*m, sizeof(double));

    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            result_mat[i][j] = a[i][j] - b[i][j];
        }
    }
    return result_mat;
}

//calc squre of Frobenius norm of mat size n X m
double squre_frob_norm(double **mat, int n, int m){
    int i, j;
    double result = 0;

    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            result += pow(mat[i][j], 2);
        }
    }

    return result;
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

//get next h by formula
double **get_next_h(double **prev_h, double **w, double beta, int n, int k){
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

double **get_h(double **init_h, double **w, int beta, int n, int k, int max_iter, double eps){
    int i = 0, exit_flag = 0;
    double **h_t = init_h, **h_t_next;

    while(i < max_iter && exit_flag == 0){
        h_t_next = get_h_next(h_t, w, beta, n, k);
        if (squre_frob_norm(diff_mat(h_t_next, h_t, n, k), n, k) < eps){
            exit_flag = 1;
        }
    }
}

int main(int argc, char **argv)
{
    return 0;
}