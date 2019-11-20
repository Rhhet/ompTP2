#include "algo.h"
#include <stdlib.h>
#include <stdio.h>

extern double times[4];

void print_added_obj(const bool *is_added, int nb_obj) {
    puts("added objects:");
    for (int i = 0; i < nb_obj; i++) 
        if (is_added[i])
            printf("%4d ", i);
    puts("");
}

void print_mat(const int *mat, int lg_nb, int cl_nb) {
    for (int i = 0; i < lg_nb; i++) {
        for (int j = 0; j < cl_nb; j++)
            printf(" %4d ", mat[i * cl_nb + j]);
        printf("\n");
    }
}

void init_util_mat(const data *pb_data, int *util_mat) {
    double start0, end0, start1, end1;
    int nb_obj = pb_data -> num_obj;
    int cap = pb_data -> capacity;
    const int *util = pb_data -> utility;
    const int *wght = pb_data -> weight;

#pragma omp parallel
    {
#ifdef THD_TRACE
        printf("thd #%d\n", omp_get_thread_num());
#endif
#pragma omp single
        start0 = omp_get_wtime();
#pragma omp for
        for (int j = 0; j < cap + 1; j++) 
            util_mat[j] = (wght[0] <= j) ? util[0] : 0;
#pragma omp single 
        {
            end0 = omp_get_wtime();
            start1 = omp_get_wtime();
        }
        for (int i = 1; i < nb_obj; i++) {
#pragma omp for
            for (int j = 0; j < cap + 1; j++) {
                // at any given time, all threads are on the same iteration 
                // of the i loop => no dependencies on old_util and new_util
                // between threads
                int old_util = util_mat[(i - 1) * (cap + 1) + j];
                if (wght[i] <= j) {
                    int new_util = util_mat[(i - 1) * (cap + 1) + j - wght[i]] + util[i];
                    // not a critical section: every thread treats the i,j coord
                    // of the matrix independently
                    if (new_util > old_util)
                        util_mat[i * (cap + 1) + j] = new_util;
                    else
                        util_mat[i * (cap + 1) + j] = old_util;
                } else
                    util_mat[i * (cap + 1) + j] = old_util;
            }
            // implicit wall for all threads => threads execute the same i loop iteration
        }
#pragma omp single
        end1 = omp_get_wtime();
    } /* omp parallel */
    times[1] = end0 - start0;
    times[2] = end1 - start1;
}

// This code cannot be multithreaded (iterations have to go in order, 
// and j is shared (and must be so))
// => not really a problem: only nb_obj iterations
int knsck_pb_alg(const data *pb_data, const int *util_mat, bool *is_added) {
    int cap = pb_data -> capacity;
    int nb_obj = pb_data -> num_obj;
    int j = cap;

    int max_util = util_mat[(nb_obj - 1) * (cap + 1) + cap];

    double start = omp_get_wtime();
    for (int i = nb_obj - 1; i > 0; i--) {        
        if (util_mat[i * (cap + 1) + j] != util_mat[(i - 1) * (cap + 1) + j]) {
            is_added[i] = true;
            j -= pb_data -> weight[i];
        } else
            is_added[i] = false;
        
    }
    is_added[0] = (util_mat[j]) ? true : false;
    double end = omp_get_wtime();
    times[3] = end - start;     // time needed to find the objects to add
    return max_util;
}
