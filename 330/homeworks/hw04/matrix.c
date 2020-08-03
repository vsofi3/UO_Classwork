#include "matrix.h"

void print_bfs_matrix_result(int rows, int *color, int *distance)
{
    assert(color);
    assert(distance);

    printf("---- Print BFS Matrix Result ----\n");
    printf("Vert\tCol\tDis\n");
    for(int i = 0; i < rows; i++) {
        printf("%d\t%d\t%d\n", i, color[i], distance[i]);
    }
    printf("--------\n\n");
}


void print_vector(int *vector, int rows)
{
    assert(vector);

    printf("---- Print Vector ----\n");
    for(int i = 0; i < rows; i++) {
        printf("%d\n", vector[i]);
    }
    printf("--------\n\n");
}

void print_matrix(int **matrix, int rows, int cols)
{
    assert(matrix);

    printf("---- Print Matrix ----\n");
    printf("This matrix is %d x %d\n", rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("--------\n\n");

}


// This function takes in a 2-D matrix, src, and transposes
// it and stores it in the 2-D matrix dst
// transpose operation takes each src[i][j] element and stores it
// in dst[j][i]
void matrix_transpose(int **dst, int **src, int rows, int cols)
{
    // Part 2 - Implement this function
}

void spmv(int **mat, int *vec, int *res, int rows, int cols)
{
    assert(mat);
    assert(vec);
    assert(res);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            res[i] += mat[i][j] * vec[j];
        }
    }
}

void color_graph(int *dst, int *color, int rows)
{
    assert(dst);
    assert(color);

    for(int i = 0; i < rows; i++) {
	// if a vertex has been visited, color it `black'
        if(dst[i] > 0) {
            color[i] = 2;
	}
    }
}
void distance_graph(int *dst, int *distance, int *color, int rows, int dis)
{
    assert(dst);
    assert(distance);
    assert(color);

    // for the neighbors for a source vertex, the distance is 
    // distancep[source] + 1 (which is also dis)
    for(int i = 0; i < rows; i++) {
        if((dst[i] > 0) && (color[i] == 0)) {
            distance[i] = dis;
	}
    }

}


int scan_vector(int *vector, int rows)
{
    assert(vector);

    int allzero = 1;
    for(int i = 0; i < rows; i++) {
        if(vector[i] != 0) {
            allzero = 0;
	}
    }
    return allzero;
}

void find_source(int *src, int *color, int rows)
{
    assert(src);
    assert(color);

    for(int i = 0; i < rows; i++) {
        // if already visited, do not add to the source
        if(color[i] != 0) {
	    src[i] = 0;
	}
	// If not visited, add to source 
	if(src[i] != 0) {
	    src[i] = 1;
	}
    }
}

int bfs_spmv(int **int_array, int rows, int cols, int source,
              int *color, int *distance)
{
    if(rows != cols) {
        printf("Not an adjacency matrix\n");
	return 0;
    }
    if(source >= rows) {
        printf("Invalid source vertex\n");
	return 0;
    }
    assert(int_array);
    assert(color);
    assert(distance);

    int **mat_trans = NULL;
    // This function initializes a 2-D array by allocating memory appropriately
    init_2d_array(&mat_trans, cols, rows);
    // This function transposes the array int_array and stores it in the array
    // mat_trans
    matrix_transpose(mat_trans, int_array, rows, cols);


    // Part 3 - Implement the rest of this function


    #if DEBUG
    print_bfs_matrix_result(rows, color, distance);
    #endif

    free_2d_array(mat_trans, cols);
    
    return 1;
}


void init_2d_array(int ***arr, int rows, int cols)
{
    // Part 1a - Implement this function

}

void free_2d_array(int **arr, int rows)
{
    // Part 1b - Implement this function
}
