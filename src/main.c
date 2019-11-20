#include <stdlib.h>
#include <stdio.h>
#include "fnct.h"
#include "algo.h"

#define MAX_THD_NB 16

int thd_nb, nb_obj;
double times[4];

static void print_times(const double *times, int nb_lines) {
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                printf("rd=%.20f\n", times[i]);
                break;
            case 1:
                printf("1st line=%.20f\n", times[i]);
                break;
            case 2:
                printf("all lines=%.20f (avrg line=%.20f)\n", times[i],
                        times[i] / (double)nb_lines);
                break;
            case 3:
                printf("added obj vec=%.20f\n", times[i]);
                break;
            default:;
        }
    }
}

static void print_alg_res(int thd_nb, data pb_data, const char *pb_filename) {
    omp_set_num_threads(thd_nb);

    double start_rd = omp_get_wtime();
    read_problem(pb_filename, &pb_data);    // reads the data from the given file
    double end_rd = omp_get_wtime();

    times[0] = end_rd - start_rd;
    int cap = pb_data.capacity;
    nb_obj = pb_data.num_obj;
    bool is_added[nb_obj];
    int *mat = malloc(sizeof(int) * nb_obj * (cap + 1));

    printf("capacity: %d  nb obj: %d\n", cap, nb_obj);

    init_util_mat(&pb_data, mat);   // initializes utility matrix
    
    // computes the best utility and the vector of added objets
    printf("best util: %d\n", knsck_pb_alg(&pb_data, mat, is_added));
    print_added_obj(is_added, nb_obj);
    free(mat);
}

int main(int argc, char **argv) {
    double start, end;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <num of threads>\n0 or negative "
                "to run in sequential mode\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int input;
    if ((input = atoi(argv[1])) <= 0) {
        puts("running in sequential");
        thd_nb = 1;
    } 
    thd_nb = (input < MAX_THD_NB) ? input : MAX_THD_NB;
    data pb_data;

    start = omp_get_wtime();
    print_alg_res(thd_nb, pb_data, "../resources/pb2.txt");
    end = omp_get_wtime();  // will give total elpased time of the program

    printf("total elapsed time: %.10f\n", end - start);    
    puts("other el times:");
    print_times(times, nb_obj);

    return EXIT_SUCCESS;
}