#include <stdlib.h>
#include <stdio.h>
#include "fnct.h"


void read_problem(const char *filename, data *pb_data) {
    char line[MAX_LINE_LEN];
    pb_data->num_obj = 0;

    FILE *problem = fopen(filename, "r");
    if (problem == NULL) {
        fprintf(stderr, "File %s not found.\n", filename);
        exit(-1);
    }

    while (fgets(line, MAX_LINE_LEN, problem) != NULL) {
        switch (line[0]) {
        case 'c': // capacity
            if (sscanf(&(line[2]), "%d\n", &(pb_data -> capacity)) != 1) {
                fprintf(stderr, "Error in file format in line:\n");
                fprintf(stderr, "%s", line);
                exit(-1);
            }
            break;

        case 'o': // graph size
            if (pb_data -> num_obj >= MAX_NUM_OBJ) {
                fprintf(stderr, "Too many objects (%d): limit is %d\n", pb_data -> num_obj, MAX_NUM_OBJ);
                exit(-1);
            }
            if (sscanf(&(line[2]), "%d %d\n", &(pb_data -> weight[pb_data -> num_obj]), 
                    &(pb_data -> utility[pb_data -> num_obj])) != 2) {
                fprintf(stderr, "Error in file format in line:\n");
                fprintf(stderr, "%s", line);
                exit(-1);
            } else
                pb_data -> num_obj++;
            break;

        default:
            break;
        }
    }
    if (pb_data -> num_obj == 0) {
        fprintf(stderr, "Could not find any object in the problem file. Exiting.");
        exit(-1);
    }
}

