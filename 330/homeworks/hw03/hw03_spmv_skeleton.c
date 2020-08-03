#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MATRIX_NAME "sparse_matrix_1.dat"
#define VECTOR_NAME "vector_1.dat"

typedef u_int32_t uint32_t;

void load_matrix(int ***int_array, uint32_t *row, uint32_t *col, uint32_t *nnz);
void load_vector(int **int_vector, uint32_t col);
void print_sparse_matrix(uint32_t row, uint32_t col, uint32_t nnz, int **arr);
void print_vector(uint32_t row, int *arr);
void SpMV_Dense(uint32_t row, uint32_t col, int **array, int *vector, int *res);
void convert_csr(uint32_t row, uint32_t col, int **int_array, uint32_t nnz, 
                 int **row_ptr, int **col_ind, int **val);
void print_csr(uint32_t row, int *row_ptr, int *col_ind, int *val);
void SpMV_CSR(uint32_t row, int *row_ptr, int *col_ind, int *val, int *vec, 
              int *res);
void convert_csr_to_coo(uint32_t row, int *row_ptr, int *col_ind, int *val, 
                        int **coo_row, int **coo_col, int **coo_val);
void print_coo(uint32_t nnz, int *coo_row, int *coo_col, int *coo_val);
void SpMV_COO(uint32_t row, uint32_t nnz, int *coo_row, int *coo_col, 
              int *coo_val, int *vec, int *res);
void verify_res(uint32_t row, int *res_truth, int *res_check);

int main(int argc, char **argv)
{
    // Load matrix
    int **int_array = NULL;
    uint32_t row = 0;
    uint32_t col = 0;
    uint32_t nnz = 0;
    load_matrix(&int_array, &row, &col, &nnz);

    // Load vector
    int *int_vector = NULL;
    load_vector(&int_vector, col);

    // This function prints the content of the matrix - debugging tools
    print_sparse_matrix(row, col, nnz, int_array);
    print_vector(col, int_vector);

    // -------------------------
    // Calculate as dense matrix
    int *res_dense = NULL;
    res_dense = (int*) malloc(sizeof(int) * row);
    if(res_dense == NULL) {
        fprintf(stderr, "Malloc error for res_dense\n");
    }
    SpMV_Dense(row, col, int_array, int_vector, res_dense);
    printf("Printing res_dense - this should be correct\n");
    print_vector(row, res_dense);
    // -------------------------

    // -------------------------
    // Convert to CSR and calculate
    printf("---- Begin Answer - CSR ----\n");
    int *row_ptr = NULL;
    int *col_ind = NULL; 
    int *val = NULL; 
    convert_csr(row, col, int_array, nnz, &row_ptr, &col_ind, &val);
//    Print the newly converted format to check for correctness - debugging tool
//    print_csr(row, row_ptr, col_ind, val);

    int *res_csr = NULL;
    res_csr = (int*) malloc(sizeof(int) * row);
    if(res_csr == NULL) {
        fprintf(stderr, "Malloc error for res_csr\n");
    }
    // Part 1 - implement SpMV_CSR
    SpMV_CSR(row, row_ptr, col_ind, val, int_vector, res_csr);
    printf("Printing res_csr\n");
    print_vector(row, res_csr);

    verify_res(row, res_dense, res_csr);
    printf("---- End Answer - CSR ----\n\n\n");
    // -------------------------

    // -------------------------
    // Conver CSR to COO and calculate
    printf("---- Begin Answer - COO ----\n");
    int *coo_row =  NULL;
    int *coo_col = NULL;
    int *coo_val = NULL;
    // Part 2 - implement convert_csr_to_coo
    convert_csr_to_coo(row, row_ptr, col_ind, val, 
                       &coo_row, &coo_col, &coo_val);
    // Print the newly converted format to check for correctness - debugging tool
     print_coo(nnz, coo_row, coo_col, coo_val);

    int *res_coo = NULL;
    res_coo = (int*) malloc(sizeof(int) * row);
    if(res_coo == NULL) {
        fprintf(stderr, "Malloc error for res_coo\n");
    }
    // Part 3- implement SpMV_COO
    SpMV_COO(row, nnz, coo_row, coo_col, coo_val, int_vector, res_coo);
    printf("Printing res_coo\n");
    print_vector(row, res_coo);

    verify_res(row, res_dense, res_coo);
    printf("---- End Answer - COO ----\n\n\n");
    // -------------------------


    // Compare results 


    // Free memory
    for(int i = 0; i < row; i++) {
        free(int_array[i]);
    }
    free(int_array);
    free(int_vector);
    free(res_dense);

    free(row_ptr);
    free(col_ind);
    free(val);
    free(res_csr);

    free(coo_row);
    free(coo_col);
    free(coo_val);
    free(res_coo);
   
    return 0;
}




void load_matrix(int ***int_array, uint32_t *row, uint32_t *col, uint32_t *nnz)
{
    FILE *fp = NULL;
    fp = fopen(MATRIX_NAME, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error while loading the file\n");
        exit(EXIT_FAILURE);
    }

    int r = 0;
    int c = 0;
    int z = 0;
    fscanf(fp, "%d", &r);
    fscanf(fp, "%d", &c);
    fscanf(fp, "%d", &z);

    int cnt = 0;
    int cnt_nz = 0;
    int tmp = 0;
    while(fscanf(fp, "%d", &tmp) == 1) {
        cnt++;
        if(tmp > 0) {
            cnt_nz++;
        }
    }
    fclose(fp);

    if((r * c == cnt) && (z == cnt_nz)) {
        printf("This is a %d x %d matrix with %d non-zeros\n", r, c, z);
    } else {
        printf("Something does not match: %d x %d = %d and %d nz == %d nz!\n",
               r, c, cnt, z, cnt_nz);
        exit(0);
    }

    int **tmp_array = (int**) malloc(sizeof(int*) * r);
    for(int i = 0; i < r; i++) {
        tmp_array[i] = (int*) malloc(sizeof(int) * c);
    }

    fp = fopen(MATRIX_NAME, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error while loading the file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &tmp);
    fscanf(fp, "%d", &tmp);
    fscanf(fp, "%d", &tmp);
 
    cnt = 0;
    tmp = 0;
    while (fscanf(fp, "%d", &tmp) == 1) {
        tmp_array[cnt / c][cnt % c] = tmp;
        cnt++;
    }
    fclose(fp);
    
    *int_array = tmp_array;
    *row = r;
    *col = c;
    *nnz = z;
}


void load_vector(int **int_vector, uint32_t col)
{
    FILE *fp = NULL;
    fp = fopen(VECTOR_NAME, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error while loading the file\n");
        exit(EXIT_FAILURE);
    }

    int cnt = 0;
    int tmp;
    while(fscanf(fp, "%d", &tmp) == 1) {
        cnt++;
    }
    if(cnt != col) {
        fprintf(stderr, "Size does not match: %d != %d\n", cnt, col);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    int *tmp_array = (int*) malloc(sizeof(int) * col);

    fp = fopen(VECTOR_NAME, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error while loading the file\n");
        exit(EXIT_FAILURE);
    }

    cnt = 0;
    while (fscanf(fp, "%d", &tmp) == 1) {
        tmp_array[cnt] = tmp;
        cnt++;
    }
    fclose(fp);

    *int_vector = tmp_array;
}

void print_sparse_matrix(uint32_t row, uint32_t col, uint32_t nnz, int **arr)
{
    printf("---- Print Array ----\n");
    printf("This matrix is %d x %d with %d nz\n", row, col, nnz);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col;j ++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("--------\n\n");
}

void print_vector(uint32_t row, int *arr)
{
    printf("---- Print Vector ----\n");
    printf("This vector is %d long\n", row);
    for(int i = 0; i < row; i++) {
        printf("%d\n", arr[i]);
    }
    printf("--------\n");
}

void SpMV_Dense(uint32_t row, uint32_t col, int **array, int *vector, int *res)
{
    for(uint32_t i = 0; i < row; i++) {
        int accum = 0;
        for(uint32_t j = 0; j < col; j++) {
            accum += array[i][j] * vector[j];
        }
        res[i] = accum;
    }
}

void convert_csr(uint32_t row, uint32_t col, int **int_array, uint32_t nnz,
                 int **row_ptr, int **col_ind, int **val)
{
    int *row_ptr_ = NULL;
    int *col_ind_ = NULL;
    int *val_= NULL;

    // We already know how large each data structure should be
    row_ptr_ = (int*) malloc(sizeof(int) * (row + 1));
    col_ind_ = (int*) malloc(sizeof(int) * nnz); 
    if(col_ind_ == NULL) {
        fprintf(stderr, "Malloc error for col_ind_\n");
        exit(EXIT_FAILURE);
    }
    val_= (int*) malloc(sizeof(int) * nnz); 
    if(val_ == NULL) {
        fprintf(stderr, "Malloc error for val_\n");
        exit(EXIT_FAILURE);
    }

    int nz_cnt = 0;
    row_ptr_[0] = 0; // first element always points towards index 0
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(int_array[i][j] > 0) {
                col_ind_[nz_cnt] = j;
                val_[nz_cnt] = int_array[i][j];
                nz_cnt++;
                if(nz_cnt > nnz) {
                    fprintf(stderr, "More nnz than we expected %d != %d\n",
                            nz_cnt, nnz);
                    exit(EXIT_FAILURE);
                }
            }
            row_ptr_[i + 1] = nz_cnt;
        }
    }



    *row_ptr = row_ptr_;
    *col_ind = col_ind_;
    *val = val_;
}

void print_csr(uint32_t row, int *row_ptr, int *col_ind, int *val)
{
    int cnt = 0;
    for(int i = 0; i < row; i++) {
        for(int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
            printf("%d - %d %d %d\n", cnt, i, col_ind[j], val[j]);
            cnt++;
        }
    }
}

void SpMV_CSR(uint32_t row, int *row_ptr, int *col_ind, int *val, int *vec, 
              int *res)
{
    // Insert your code here (1)
	
	for(int i = 0; i < row; i++) 
	{
		for(int j = row_ptr[i]; j < row_ptr[i + 1]; j++) 
		{
			res[i] += val[j] * vec[col_ind[j]];	
		}
	}
}		

void convert_csr_to_coo(uint32_t row, int *row_ptr, int *col_ind, int *val, 
                        int **coo_row, int **coo_col, int **coo_val)
{
    // Insert your code here (2)
	int nonZero = 0; 
	int size = sizeof(int); // helps minimize calculations 
	for(int i = 0; i < row; i++)
	{
		for(int j = row_ptr[i]; j < row_ptr[i+1]; j++)
		{
			nonZero++; //need to know how many ints will be in the list
		}
	}
	int raySize = size * nonZero; // minimizes repetetive calculations 
	*coo_row = malloc(raySize); // allocate space for each of the arrays used in the coo format
	*coo_col = malloc(raySize); 
	*coo_val = malloc(raySize);
	
	
	for(int i = 0; i < row; i++)
	{
		for(int j = row_ptr[i]; j < row_ptr[i+1]; j++)
		{
			(*coo_row)[j] = i;		//set coo_row, coo_col, and coo_val  
			(*coo_col)[j] = col_ind[j];  	
			(*coo_val)[j] = val[j];
		}
	}
	 
}

void print_coo(uint32_t nnz, int *coo_row, int *coo_col, int *coo_val)
{
    int cnt = 0;
    for(int i = 0; i < nnz; i++) {
        printf("%d %d %d %d\n", cnt, coo_row[i], coo_col[i], coo_val[i]);
        cnt++;
    }
}


void SpMV_COO(uint32_t row, uint32_t nnz, int *coo_row, int *coo_col, 
              int *coo_val, int *vec, int *res)
{
    // Insert your code here (3)
	for(int i = 0; i < nnz; i++)
	{
		res[coo_row[i]] += vec[coo_col[i]] * coo_val[i];
	}
}

void verify_res(uint32_t row, int *res_truth, int *res_check)
{
    int wrong = 0;
    for(int i = 0; i < row; i++) {
        if(res_truth[i] != res_check[i]) {
            wrong++;
        }
    }
    printf("We found %d/%d incorrect answers\n", wrong, row);
}

