#include "common.h"

void verify_answer_adj_list(adj_node_t **ans, adj_node_t **in, int rows)
{
    printf("---- ADJ LIST CHECK ----\n");
    assert(ans);
    int cnt = 0;
    for(int i = 0; i < rows; i++) {
        adj_node_t* next = ans[i];
        while(next != NULL) {
            cnt++;
            next = next->next;
        }
    }

    if(in == NULL) {
        printf("0/%d is correct for adjacency list\n", cnt);
    } else {
        int err = 0;
        for(int i = 0; i < rows; i++) {
            adj_node_t *next_ans = ans[i];
            adj_node_t *next_in = in[i];
            while((next_ans != NULL)) {
                if(next_in != NULL) {
                    if(next_ans->vid != next_in->vid) {
                        err++;
                    }
                    next_in = next_in->next;
                } else {
                    err++;
                }
                next_ans = next_ans->next;
            }
        }
        printf("%d/%d is correct for adjacency list\n", (cnt - err), cnt);
    }
    printf("--------\n\n");
}

void verify_answer_bfs(int *col_ans, int *dis_ans, int *par_ans, 
                       int *col_in, int *dis_in, int *par_in, int rows)
{
    printf("---- BFS CHECK ----\n");
    assert(col_ans);
    assert(dis_ans);
    assert(par_ans);
    assert(col_in);
    assert(dis_in);
    assert(par_in);

    int err_col = 0;
    int err_dis = 0;
    int err_par = 0;
    for(int i = 0; i < rows; i++) {
        if(col_ans[i] != col_in[i]) {
            err_col++;
        }
        if(dis_ans[i] != dis_in[i]) {
            err_dis++;
        }
        if(par_ans[i] != par_in[i]) {
            err_par++;
        } 
    }
    printf("%d/%d is correct for color\n", (rows - err_col), rows);
    printf("%d/%d is correct for distance\n", (rows - err_dis), rows);
    printf("%d/%d is correct for parents\n", (rows - err_par), rows);
    printf("--------\n\n");
}

void verify_answer_bfs_spmv(int *col_ans, int *dis_ans, 
                            int *col_in, int *dis_in, int rows)

{
    printf("---- BFS SPMV CHECK ----\n");
    assert(col_ans);
    assert(dis_ans);
    assert(col_in);
    assert(dis_in);

    int err_col = 0;
    int err_dis = 0;
    for(int i = 0; i < rows; i++) {
        if(col_ans[i] != col_in[i]) {
            err_col++;
        }
        if(dis_ans[i] != dis_in[i]) {
            err_dis++;
        }
    }
    printf("%d/%d is correct for color\n", (rows - err_col), rows);
    printf("%d/%d is correct for distance\n", (rows - err_dis), rows);
    printf("--------\n\n");
}
