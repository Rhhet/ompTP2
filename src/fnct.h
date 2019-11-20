#ifndef FNCT_H
#define FNCT_H

#define MAX_NUM_OBJ 100
#define MAX_LINE_LEN 256  // Max size of a line to be read (1 byte)


typedef struct {
    int num_obj;
    int capacity;
    int weight[MAX_NUM_OBJ];
    int utility[MAX_NUM_OBJ];
} data;


void read_problem(const char *filename, data *data_pb);


#endif