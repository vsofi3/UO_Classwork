#ifndef COMMON_H_
#define COMMON_H_
#ifndef DEBUG
#define DEBUG 0
#endif
#include "graph.h"

void verify_answer_adj_list(adj_node_t **ans, adj_node_t **in, int rows);
void verify_answer_bfs(int *col_ans, int *dis_ans, int *par_ans, 
                       int *col_in, int *dis_in, int *par_in, int rows);
void verify_answer_bfs_spmv(int *col_ans, int *dis_ans, 
                            int *col_in, int *dis_in, int rows);

#endif
