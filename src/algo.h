#ifndef ALGO_H
#define ALGO_H

#include <stdbool.h>
#include <omp.h>
#include "fnct.h"

#define THD_NUM     // thread number


// Prints the objects contained in the knapsack according
// to the boolean array is_added
void print_added_obj(const bool *is_added, int nb_obj);

// Prints a matrix
void print_mat(const int* mat, int lg_nb, int cl_nb);

// Initializes the utility matrix for the given data
void init_util_mat(const data *pb_data, int *util_mat);

// Knapsack problem algorithm
int knsck_pb_alg(const data *pb_data, const int *util_mat, bool *is_added);

#endif