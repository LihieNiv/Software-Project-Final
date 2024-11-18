#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*NEED TO DO:
 *Add shell functions for sym/ddg/norm
 *Add conversion function mat->vec, vec->mat - DONE
 *Finish reading file into MATRIX (read number of "\n","," to find it)
 *Find out the most reasonable way to read the numbers.
 */

// Make sure all numbers use double
long long total_mem = 0;
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

double **vector_to_matrix(vector *p, int n, int k)
{
    cord *p_c = p->cords;
    double **ret_mat = (double **)calloc(n * k, sizeof(double));
    int i, j;
    total_mem += n * k * sizeof(double);
    i = 0;
    while (p != NULL)
    {
        p_c = p->cords;
        j = 0;
        while (p_c != NULL)
        {
            ret_mat[i][j] = p_c->value;
            p_c = p_c->next;
            j += 1;
        }
        i += 1;
    }
    return ret_mat;
}

vector *matrix_to_vector(double **mat, int n, int k)
{
    int i, j;
    cord *p_c;
    vector *v_p;
    vector *head;
    v_p = (vector *)malloc(sizeof(vector));
    head = v_p;
    for (i = 0; i < n; i++)
    {
        p_c = (cord *)nalloc(sizeof(cord));
        v_p->cords = p_c;
        for (j = 0; j < k; j++)
        {
            p_c->value = mat[i][j];
            if (j != k - 1)
            {
                p_c->next = (cord *)malloc(sizeof(cord));
                p_c = p_c->next;
            }
        }
        if (i != n - 1)
        {
            v_p->next = (vector *)malloc(sizeof(vector));
            v_p = v_p->next;
        }
    }
    return head;
}
// Computes diagonal degree matrix
vector *ddg(vector *data, int n)
{
    double **ret_mat = ddg_comp(data, n);
    return matrix_to_vector(ret_mat, n, n);
}
double **ddg_comp(vector *data, int n)
{
    double **ret = (double **)calloc(n * n, sizeof(double));
    double **sym_mat = sym_comp(data, n);
    int i;
    double *diag = diag_deg(sym_mat, n);
    total_mem += n * n * sizeof(double);
    for (int i = 0; i < n; i++)
    {
        ret[i][i] = diag[i];
    }
    total_mem -= sizeof(sym_mat);
    total_mem -= sizeof(diag);
    free(sym_mat);
    free(diag);
    return ret;
}
// Computes diagonal degree vector (just the diagonal)
double *diag_deg(double **sym_mat, int n)
{
    double *ret = (double *)calloc(n, sizeof(double));
    int i, j;
    total_mem += n * sizeof(double);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            ret[i] += sym_mat[i][j];
        }
    }
    return ret;
}
// Computes similarity between two vectors
double euc_dist(cord *x, cord *y)
{
    double ret = 0;
    if (x == NULL || y == NULL)
    {
        // RAISE AN ERROR - NULL VECTOR
        return 1;
    }
    while (x->next != NULL)
    {
        if (y->next == NULL)
        {
            // RAISE AN ERROR - DIFFERENT LENGTHS
            return 1;
        }
        ret += (x->value - y->value) * (x->value - y->value);
        x = x->next;
        y = y->next;
    }
    ret += (x->value - y->value) * (x->value - y->value);
    return ret;
}

// Computes similarity matrix
vector *sym(vector *data, int n)
{
    double **ret_mat = sym_comp(data, n);
    return matrix_to_vector(ret_mat, n, n);
}

double **sym_comp(vector *data, int n)
{
    double **ret = (double **)calloc(n * n, sizeof(double));
    int i, j;
    total_mem += n * n * sizeof(double);
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            ret[i][j] += exp(-0.5 * euc_dist(data[i].cords, data[j].cords));
            ret[j][i] = ret[i][j];
        }
        ret[i][i] = 0;
    }
    return ret;
}

// Computes normalized similarity matrix
vector *norm(vector *data, int n)
{
    double **ret_mat = norm_comp(data, n);
    return matrix_to_vector(ret_mat, n, n);
}

double **norm_comp(vector *data, int n)
{
    double **ret = (double **)calloc(n * n, sizeof(double));
    double **sym_mat = sym_comp(data, n);
    double *diag = diag_deg(sym_mat, n);
    int i, j;
    total_mem += n * n * sizeof(double);
    for (i = 0; i < n; i++)
    {
        diag[i] = pow(diag[i], -0.5);
    }
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
        {
            ret[i][j] = diag[i] * sym_mat[i][j] * diag[j];
            ret[j][i] = ret[i][j];
        }
    }
    total_mem -= sizeof(diag);
    total_mem += sizeof(sym_mat);
    free(diag);
    free(sym_mat);
    return ret;
}
// Matrix A: k*n, Matrix B: n*m, Matrix A*B: k*m
double **mult_mat(double **matA, double **matB, int n, int k, int m)
{
    double **ret = (double **)calloc(k * m, sizeof(double));
    int i, j, p, sum;
    total_mem += k * m * sizeof(double);
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
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%.4f", mat[i][j]);
            if (j != m - 1)
            {
                printf(","); // CHECK THAT SEPARATOR IS ","
            }
        }
        printf("\n");
    }
}

cord *read_line(FILE *file)
{
    cord *head, *cur;
    head = NULL;
    char c;
    double val;
    int flag = 1;
    int count = 0;
    while ((c = getc(file)) != '\n')
    {
        if (c == ',')
        {
            cur->value = val;
            val = 0;
            cur->next = (cord *)malloc(sizeof(cord));
            total_mem += sizeof(cord);
            cur = cur->next;
            flag = 1;
            continue;
        }
        if (head == NULL)
        {
            head = (cord *)malloc(sizeof(cord));
            cur = head;
            total_mem += sizeof(cord);
        }
        if (c == '.')
        {
            if (flag == 0)
            {
                // RAISE ERROR - %..% number
                exit(1);
            }
            flag = 0;
            count = 1;
            continue;
        }
        if (c <= '0' || c >= '9')
        {
            // RAISE ERROR - BAD NUMBER
            exit(1);
        }
        if (flag == 1)
        {
            val *= 10;
            val += c - '0';
            continue;
        }
        val += c * pow(10, -1 * count);
        count++;
    }
    return head;
}

int main(int argc, char **argv)
{
    char *goal, *path;
    vector *data = NULL;
    vector **more_data = NULL;
    cord *cur_cord = NULL;
    vector *cur_vec = NULL;
    FILE *fptr = NULL;
    char new_c;
    int n = 0;
    if (argc != 3)
    {
        // ERROR - WRONG NUMBER OF PARAMETERS.
        return 1;
    }
    goal = argv[1];
    path = argv[2];
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        // ERROR - FAILED TO OPEN FILE
        return 1;
    }
    while (getc(fptr) != EOF)
    {
        // NEED TO FINISH
    }
}