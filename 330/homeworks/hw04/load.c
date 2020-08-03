#include "load.h"

void load_matrix(char *mat_name, int ***int_array, 
                 uint32_t *row, uint32_t *col, uint32_t *nnz)
{
    FILE *fp = NULL;
    fp = fopen(mat_name, "r");
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
        exit(EXIT_FAILURE);
    }

    int **tmp_array = (int**) malloc(sizeof(int*) * r);
    assert(tmp_array);
    for(int i = 0; i < r; i++) {
        tmp_array[i] = (int*) malloc(sizeof(int) * c);
        assert(tmp_array[i]);
    }

    fp = fopen(mat_name, "r");
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

