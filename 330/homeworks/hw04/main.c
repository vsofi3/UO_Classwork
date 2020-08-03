#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "common.h"
#include "load.h"
#include "matrix.h"
#include "graph.h"
#if TEST
#include "ans.h"
#endif

void usage(int argc, char **argv)
{
    if(argc != 3) {
        printf("usage: %s <matrix file> <source vertex>\n", argv[0]);
        exit(EXIT_FAILURE);
    } 
}


int main(int argc, char **argv) 
{
    usage(argc, argv);


    // load the adjacency matrix from file
    int **int_array = NULL;
    uint32_t row = 0;
    uint32_t col = 0;
    uint32_t nnz = 0;
    load_matrix(argv[1], &int_array, &row, &col, &nnz);
    assert((row > 0) && (col > 0));
    #if 1
    print_matrix(int_array, row, col);
    #endif
    // Read the source vertex for BFS
    int src = atoi(argv[2]);


    // Construct an adjacency list for the graph
    // adj_list is an array of size `row,' and each element in this array
    //     points to the head of each row's adjacency list 
    adj_node_t **adj_list = NULL;
    construct_adj_list(int_array, row, col, &adj_list);
    #if DEBUG
    print_adj_list(adj_list, row);
    #endif


    // BFS
    int *color = (int*) malloc(sizeof(int) * row);
    assert(color);
    memset(color, -2, sizeof(int) * row);
    int *distance = (int*) malloc(sizeof(int) * row);
    assert(distance);
    memset(distance, -2, sizeof(int) * row);
    int *parent = (int*) malloc(sizeof(int) * row);
    assert(parent);
    memset(parent, -2, sizeof(int) * row);

    // List based BFS
    bfs(adj_list, row, src, color, distance, parent);


    // Matrix-based BFS
    // Declare and allocate the color and distance arrays
    // Parent is not required for the SpMV BFS algorithm
    int *color_mat = (int*) malloc(sizeof(int) * row);
    assert(color_mat);
    memset(color_mat, -2, sizeof(int) * row);
    int *distance_mat = (int*) malloc(sizeof(int) * row);
    assert(distance_mat);
    memset(distance_mat, -2, sizeof(int) * row);

    bfs_spmv(int_array, row, col, src, color_mat, distance_mat);




    #if TEST
    // Adj list construction
    adj_node_t **adj_list_ans = NULL;
    construct_adj_list_ans(int_array, row, col, &adj_list_ans);
    #if DEBUG
    print_adj_list(adj_list_ans, row);
    #endif

    // BFS
    int *color_ans = (int*) malloc(sizeof(int) * row);
    assert(color_ans);
    int *distance_ans = (int*) malloc(sizeof(int) * row);
    assert(distance_ans);
    int *parent_ans = (int*) malloc(sizeof(int) * row);
    assert(parent_ans);

    // List based BFS
    bfs_ans(adj_list_ans, row, src, color_ans, distance_ans, parent_ans);

    // BFS SpMV
    int *color_mat_ans = (int*) malloc(sizeof(int) * row);
    assert(color_mat_ans);
    int *distance_mat_ans = (int*) malloc(sizeof(int) * row);
    assert(distance_mat_ans);

    bfs_spmv_ans(int_array, row, col, src, color_mat_ans, distance_mat_ans);


    // Check answers
    verify_answer_adj_list(adj_list_ans, adj_list, row);
    verify_answer_bfs(color_ans, distance_ans, parent_ans,
                      color, distance, parent, row);
    verify_answer_bfs_spmv(color_mat_ans, distance_mat_ans,
                           color_mat, distance_mat, row);


    free_adj_list_ans(adj_list_ans, row);
    free(color_ans);
    free(distance_ans);
    free(parent_ans);
    free(color_mat_ans);
    free(distance_mat_ans);
    #endif


    // Clean up after your memory
    free_2d_array(int_array, row);
    free_adj_list(adj_list, row);
    free(color_mat);
    free(distance_mat);
    free(color);
    free(distance);
    free(parent);

    return 0;
}


