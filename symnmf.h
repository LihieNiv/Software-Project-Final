#ifndef SYMNMF_H
#define SYMNMF_H

#include <stdlib.h>

// Define the struct vector
struct cord {
    double value;
    struct cord *next;
};

struct vector {
    struct vector *next;
    struct cord *cords;
};

/**
 * Function: ddg
 * ----------------
 * Computes the diagonal degree matrix from the input data.
 * 
 * @param data - Pointer to the input vector data.
 * @param n    - Number of rows or size of the input data.
 * @return     - A pointer to a dynamically allocated vector representing the diagonal degree matrix.
 */
struct vector *ddg(struct vector *data, int n);

/**
 * Function: sym
 * ----------------
 * Computes a symmetric matrix from the input data.
 * 
 * @param data - Pointer to the input vector data.
 * @param n    - Size of the input data (number of elements).
 * @return     - A pointer to a dynamically allocated vector representing the symmetric matrix.
 */
struct vector *sym(struct vector *data, int n);

/**
 * Function: norm
 * ----------------
 * Computes a normalized vector based on the input data.
 * 
 * @param data - Pointer to the input vector data.
 * @param n    - Size of the input data (number of elements).
 * @return     - A pointer to a dynamically allocated vector representing the normalized data.
 */
struct vector *norm(struct vector *data, int n);

/**
 * Function: symnmf
 * ----------------
 * Solves the Symmetric Non-negative Matrix Factorization (SymNMF) problem.
 * 
 * @param H_i - Pointer to the initial vector (H) for factorization.
 * @param W   - Pointer to the similarity matrix (W).
 * @param n   - Size of the input data (number of elements).
 * @param k   - Number of latent factors for factorization.
 * @return    - A pointer to a dynamically allocated vector representing the factorization result (H).
 */
struct vector *symnmf(struct vector *H_i, struct vector *W, int n, int k);

#endif // SYMNMF_H
