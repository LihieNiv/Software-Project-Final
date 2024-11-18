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
    struct cord *next;
} cord;

typedef struct vector
{
    struct vector *next;
    cord *cords;
} vector;

double **ddg_comp(vector *data, int n);
double *diag_deg(double **sym_mat, int n);
double **sym_comp(vector *data, int n);
double **norm_comp(vector *data, int n);
vector *ddg(vector *data, int n);
vector *sym(vector *data, int n);
vector *norm(vector *data, int n);
int get_length_of_vector(vector *p);
int get_length_of_coords(cord *p);
double **vector_to_matrix(vector *p);
vector *matrix_to_vector(double **mat, int n, int k);
double **mult_mat(double **matA, double **matB, int n, int k, int m);
void print_mat(double **mat, int n, int m);

int get_length_of_vector(vector *p)
{
    int i = 0;
    while (p != NULL)
    {
        p = p->next;
        i++;
    }
    return i;
}
int get_length_of_coords(cord *p)
{
    int i = 0;
    while (p != NULL)
    {
        i++;
        p = p->next;
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
    int n, k;
    n = get_length_of_vector(p);
    k = get_length_of_coords(p_c);
    ret_mat = (double **)calloc(n, sizeof(double *)); // Fix memory allocation
    for (int i = 0; i < n; i++)
    {
        ret_mat[i] = (double *)calloc(k, sizeof(double)); // Allocate each row
    }
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

// Computes Euclidean Distance between two arrays.
double euc_dist(double *x, double *y, int k)
{
    int i;
    double ret;
    ret = 0;
    for (i = 0; i < k; i++)
    {
        ret += pow(x[i] - y[i], 2);
    }
    // printf("%.4f\n", ret);
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
    double **ret = (double **)malloc(n * sizeof(double *));
    int i, j, k;
    double **mat_data = vector_to_matrix(data);
    k = get_length_of_coords(data->cords);
    // printf("Entered! n=%d\n", n);
    if (ret == NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++)
    {
        ret[i] = (double *)calloc(n, sizeof(double)); // Allocate each row
    }
    total_mem += n * n * sizeof(double);
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            ret[i][j] += exp(-0.5 * euc_dist(mat_data[i], mat_data[j], k));
            ret[j][i] = ret[i][j];
        }
        ret[i][i] = 0;
    }
    // printf("Finally!\n");
    return ret;
}

// Computes diagonal degree matrix
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
        ret[i] = (double *)calloc(n, sizeof(double)); // Allocate each row
    }
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

// Computes normalized similarity matrix
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
        ret[i] = (double *)calloc(n, sizeof(double)); // Allocate each row
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

/*cord *read_line(FILE *file)
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
*/

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
            return 0; // strings are not equal
        }
        str1++;
        str2++;
    }
    // If one string ends before the other, they are not equal
    return *str1 == *str2; // return 1 if equal, 0 if not
}

// calc A - B, size n X m each.
double **diff_mat(double **a, double **b, int n, int m)
{
    int i, j;
    double **result_mat = calloc(n * m, sizeof(double));

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            result_mat[i][j] = a[i][j] - b[i][j];
        }
    }
    return result_mat;
}

// calc squre of Frobenius norm of mat size n X m
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

// recieve mat (nXm)
// return transposed mat (mXn)
double **mat_transpose(double **mat, int n, int m)
{
    int i, j;
    double **trans_mat = calloc(m * n, sizeof(double));
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            trans_mat[j][i] = mat[i][j];
        }
    }
    return trans_mat;
}

// get next h by formula
double **get_next_h(double **prev_h, double **w, double beta, int n, int k)
{
    int i, j;
    double **new_h = calloc(n * k, sizeof(double));
    double **numerator = mult_mat(w, prev_h, n, n, k);
    double **h_on_ht = mult_mat(prev_h, mat_transpose(prev_h, n, k), n, k, n);
    ; // temp to calc the denominator
    double **denominator = mult_mat(h_on_ht, prev_h, n, n, k);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < k; j++)
        {
            new_h[i][j] = (1 - beta + beta * (numerator[i][j] / denominator[i][j]));
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
        h_t_next = get_h_next(h_t, w, beta, n, k);
        if (squre_frob_norm(diff_mat(h_t_next, h_t, n, k), n, k) < eps)
        {
            exit_flag = 1;
        }
    }
}

int main(int argc, char **argv)
{
    char *goal, *path;
    vector *data = NULL;
    cord *cur_cord = NULL;
    vector *cur_vec = NULL;
    FILE *fptr = NULL;
    double **ret_mat;
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
        printf("In here!\n");
        ret_mat = sym_comp(data, n);
        print_mat(ret_mat, n, n);
    }
    if (compare_strings(goal, "norm") == 1)
    {
        ret_mat = norm_comp(data, n);
        print_mat(ret_mat, n, n);
    }
    /*double **mat = vector_to_matrix(data);
    int k = get_length_of_coords(data->cords);
    printf("n=%d, k=%d\n", n, k);
    print_mat(mat, n, k);*/
}