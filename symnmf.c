#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*NEED TO DO:
 *Add shell functions for sym/ddg/norm
 *Add conversion function mat->vec, vec->mat - DONE
 *Finish reading file into MATRIX (read number of "\n","," to find it)
 *Find out the most reasonable way to read the numbers.
 */

long total_mem = 0;
typedef struct cord
{
    double value;
    struct cord *next;
} cord;

typedef struct vector
{
    struct vector *next;
    cord *cords;
} vector;

// calc square euclidean distsance
double euc_dst(struct vector *x, struct vector *y)
{
    double sum = 0.0;
    struct cord *curr_cord_x = x->cords;
    struct cord *curr_cord_y = y->cords;
    while (curr_cord_x != NULL)
    {
        sum += pow(curr_cord_x->value - curr_cord_y->value, 2);
        curr_cord_x = curr_cord_x->next;
        curr_cord_y = curr_cord_y->next;
    }
    return i;
}

void delete_last_vec(vector *p, int n)
{
    while (n > 1)
    {
        p = p->next;
        n--;
    }
    p->next = NULL;
}

double **vector_to_matrix(vector *p)
{
    cord *p_c = p->cords;
    double **ret_mat;
    int n, k, i, j;
    n = get_length_of_vector(p);
    k = get_length_of_coords(p_c);
    ret_mat = (double **)calloc(n, sizeof(double *));
    for (i = 0; i < n; i++)
    {
        ret_mat[i] = (double *)calloc(k, sizeof(double));
    }
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
            j++;
        }
        i++;
        p = p->next;
    }
    return ret_mat;
}

vector *matrix_to_vector(double **mat, int n, int k)
{
    int i, j;
    cord *p_c, *cur_cord;
    vector *v_p, *head;

    v_p = (vector *)malloc(sizeof(vector));
    head = v_p;
    for (i = 0; i < n; i++)
    {
        p_c = (cord *)malloc(sizeof(cord));
        v_p->cords = p_c;
        cur_cord = p_c;
        for (j = 0; j < k; j++)
        {
            cur_cord->value = mat[i][j];
            if (j != k - 1)
            {
                cur_cord->next = (cord *)malloc(sizeof(cord));
                cur_cord = cur_cord->next;
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

/*
Computes Euclidean Distance between two arrays.
*/
double euc_dist(double *x, double *y, int k)
{
    int i;
    double ret;
    ret = 0;
    for (i = 0; i < k; i++)
    {
        ret += pow(x[i] - y[i], 2);
    }
    return ret;
}

/*
Computes similarity matrix
*/
vector *sym(vector *data, int n)
{
    double **ret_mat = sym_comp(data, n);
    return matrix_to_vector(ret_mat, n, n);
}

double **sym_comp(vector *data, int n)
{
    double **ret = (double **)malloc(n * sizeof(double *));
    int i, j, k;
    double **mat_data = vector_to_matrix(data);
    k = get_length_of_coords(data->cords);

    if (ret == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++)
    {
        ret[i] = (double *)calloc(n, sizeof(double));
    }
    total_mem += n * n * sizeof(double);
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            ret[i][j] = exp(-0.5 * euc_dist(mat_data[i], mat_data[j], k));
            ret[j][i] = ret[i][j];
        }
        ret[i][i] = 0;
    }
    return ret;
}

/*
Computes diagonal degree matrix
*/
vector *ddg(vector *data, int n)
{
    double **ret_mat = ddg_comp(data, n);
    return matrix_to_vector(ret_mat, n, n);
}
double **ddg_comp(vector *data, int n)
{

    double **ret = (double **)malloc(n * sizeof(double *));
    double **sym_mat = sym_comp(data, n);
    int i;
    double *diag = diag_deg(sym_mat, n);
    total_mem += n * n * sizeof(double);
    if (ret == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++)
    {
        ret[i] = (double *)calloc(n, sizeof(double));
    }
    for (i = 0; i < n; i++)
    {
        ret[i][i] = diag[i];
    }
    total_mem -= sizeof(sym_mat);
    total_mem -= sizeof(diag);
    free(sym_mat);
    free(diag);
    return ret;
}

/*
Computes diagonal degree vector (just the diagonal)
*/
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

/*
Computes normalized similarity matrix
*/
vector *norm(vector *data, int n)
{
    double **ret_mat = norm_comp(data, n);
    return matrix_to_vector(ret_mat, n, n);
}

double **norm_comp(vector *data, int n)
{
    double **ret = (double **)calloc(n, sizeof(double *));
    double **sym_mat = sym_comp(data, n);
    double *diag = diag_deg(sym_mat, n);
    int i, j;
    total_mem += n * n * sizeof(double);
    if (ret == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++)
    {
        ret[i] = (double *)calloc(n, sizeof(double));
    }
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
/*
Matrix A: k*n, Matrix B: n*m, Matrix A*B: k*m
*/
double **mult_mat(double **matA, double **matB, int n, int k, int m)
{
    int i, j, p;
    double sum = 0;
    double **ret = (double **)calloc(k, sizeof(double *));
    for (i = 0; i < k; i++)
    {
        ret[i] = (double *)calloc(m, sizeof(double));
    }
    if (ret == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
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

/*
Prints a matrix of size n*m
*/
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
                printf(",");
            }
        }
        printf("\n");
    }
}

vector *read_file(FILE *file)
{
    vector *head_vec, *curr_vec;
    cord *head_cord, *curr_cord;
    double n;
    char c;
    head_cord = malloc(sizeof(struct cord));
    if (head_cord == NULL)
        exit(EXIT_FAILURE);
    head_cord->next = NULL;
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(vector));
    if (head_vec == NULL)
        exit(EXIT_FAILURE);
    head_vec->next = NULL;
    curr_vec = head_vec;
    curr_vec->next = NULL;
    while (fscanf(file, "%lf%c", &n, &c) == 2)
    {
        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = (vector *)malloc(sizeof(vector));
            if (curr_vec->next == NULL)
                exit(EXIT_FAILURE);
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(cord));
            if (head_cord == NULL)
                exit(EXIT_FAILURE);
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }
        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(cord));
        if (curr_cord->next == NULL)
            exit(EXIT_FAILURE);
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    return head_vec;
}

int compare_strings(const char *str1, const char *str2)
{
    while (*str1 != '\0' && *str2 != '\0')
    {
        if (*str1 != *str2)
        {
            return 0; /*strings are not equal*/
        }
        str1++;
        str2++;
    }
    /* If one string ends before the other, they are not equal */
    return *str1 == *str2; /*return 1 if equal, 0 if not*/
}

/*
calc A - B, size n X m each.
*/
double **diff_mat(double **a, double **b, int n, int m)
{
    int i, j;
    double **result_mat = (double **)calloc(n, sizeof(double *));
    for (i = 0; i < n; i++)
    {
        result_mat[i] = (double *)calloc(m, sizeof(double));
    }
    if (result_mat == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            result_mat[i][j] = a[i][j] - b[i][j];
        }
    }
    return result_mat;
}

/*
calc squre of Frobenius norm of mat size n X m
*/
double squre_frob_norm(double **mat, int n, int m)
{
    int i, j;
    double result = 0;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            result += pow(mat[i][j], 2);
        }
    }
    return result;
}

/*
recieve mat (nXm)
return transposed mat (mXn)
*/
double **mat_transpose(double **mat, int n, int m)
{
    int i, j;
    double **trans_mat = (double **)calloc(m, sizeof(double *));
    for (i = 0; i < n; i++)
    {
        trans_mat[i] = (double *)calloc(n, sizeof(double));
    }
    if (trans_mat == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            trans_mat[j][i] = mat[i][j];
        }
    }
    return trans_mat;
}

/*
get next h by formula
*/
double **get_next_h(double **prev_h, double **w, double beta, int n, int k)
{
    int i, j;
    double **numerator, **h_on_ht, **denominator, **new_h = (double **)calloc(n, sizeof(double *));
    for (i = 0; i < n; i++)
    {
        new_h[i] = (double *)calloc(k, sizeof(double));
    }
    if (new_h == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    numerator = mult_mat(w, prev_h, n, n, k);
    h_on_ht = mult_mat(prev_h, mat_transpose(prev_h, n, k), k, n, n); /*temp to calc the denominator*/
    denominator = mult_mat(h_on_ht, prev_h, n, n, k);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < k; j++)
        {
            new_h[i][j] = prev_h[i][j] * (1 - beta + beta * (numerator[i][j] / denominator[i][j]));
        }
    }
    return new_h;
}

double **get_h(double **init_h, double **w, int beta, int n, int k, int max_iter, double eps)
{
    int i = 0, exit_flag = 0;
    double **h_t = init_h, **h_t_next;

    while (i < max_iter && exit_flag == 0)
    {
        h_t_next = get_next_h(h_t, w, beta, n, k);
        if (squre_frob_norm(diff_mat(h_t_next, h_t, n, k), n, k) < eps)
        {
            exit_flag = 1;
        }
        free(h_t);
        h_t = h_t_next;
    }
    return h_t;
}

vector *symnmf(vector *H_i, vector *W, int n, int k)
{
    double **h = vector_to_matrix(H_i);
    double **w = vector_to_matrix(W);
    double **ret_mat = get_h(h, w, 0.5, n, k, 300, 0.0001);
    return matrix_to_vector(ret_mat, n, k);
}

int main(int argc, char **argv)
{
    char *goal, *path;
    vector *data = NULL;
    FILE *fptr = NULL;
    double **ret_mat;
    int n = 0;
    if (argc != 3)
    {
        /*ERROR - WRONG NUMBER OF PARAMETERS.*/
        return 1;
    }
    goal = argv[1];
    path = argv[2];
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        /*ERROR - FAILED TO OPEN FILE*/
        return 1;
    }
    data = read_file(fptr);
    n = get_length_of_vector(data);
    delete_last_vec(data, n);
    n--;
    printf("goal: %s\n", goal);
    if (compare_strings("ddg", goal) == 1)
    {
        ret_mat = ddg_comp(data, n);
        print_mat(ret_mat, n, n);
    }
    if (compare_strings("sym", goal) == 1)
    {
        ret_mat = sym_comp(data, n);
        print_mat(ret_mat, n, n);
    }
    if (compare_strings(goal, "norm") == 1)
    {
        ret_mat = norm_comp(data, n);
        print_mat(ret_mat, n, n);
    }
    return 0;
    /*double **mat = vector_to_matrix(data);
    int k = get_length_of_coords(data->cords);
    printf("n=%d, k=%d\n", n, k);
    print_mat(mat, n, k);*/
}
