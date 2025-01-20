#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#define EPS 1e-6

bool is_close(double a, double b) {
    return fabs(a - b) < EPS;
}

// Read a vector file
void read_vec_file(int n, int vec_num, double* vec) {
    char vec_file_name[100];
    sprintf(vec_file_name, "inputfiles/vec_%06d_%06d.in", n, vec_num);
    FILE *vec_file = fopen(vec_file_name, "r");

    if (vec_file == NULL) {
        perror("Error opening vector file");
        exit(1);
    }

    for(int j = 0; j < n; j++) {
        fscanf(vec_file, "%lf,", &vec[j]);
    }
    fclose(vec_file);
}

// Append the eigenvalue to a vector file
void append_eig_val(int n, int vec_num, double eig_val) {
    char vec_file_name[100];
    sprintf(vec_file_name, "inputfiles/vec_%06d_%06d.in", n, vec_num);
    FILE *vec_file = fopen(vec_file_name, "a");

    if (vec_file == NULL) {
        perror("Error opening vector file");
        exit(1);
    }

    fprintf(vec_file, "\n%lf", eig_val);
    fclose(vec_file);
}

// Check if vec is an eigenvector of mat. If yes, return true and store the eigenvalue in eig_val
void get_eig(int n, double** mat, double* vec, bool* is_eig, double* eig_val) {
    int i, j;

    // Ax = λx
    *is_eig = true;

    bool eig_val_set = false; // To track if eig_val is set

    for (i = 0; i < n; i++) {
        double sum = 0.0;
        for (j = 0; j < n; j++) {
            sum += mat[i][j] * vec[j];
        }

        if (is_close(vec[i], 0.0)) {
            // Edge case when vec[i] is 0
            if (!is_close(sum, 0.0)) {
                *is_eig = false;
                break;
            }
        }
        else {
            double lambda = sum / vec[i];
            if (!eig_val_set) {
                // First possible eigenvalue
                *eig_val = lambda;
                eig_val_set = true;
            }
            else if (!is_close(lambda, *eig_val)) {
                *is_eig = false;
                break;
            }
        }
    }

    if (!eig_val_set) {
        // Eigen value is never set, this means vec is a zero vector
        *is_eig = false;
    }

    return;
}

int main() {
    int n, i, j;

    // Read n from input.in
    FILE *input = fopen("input_ques2.in", "r");
    fscanf(input, "%d", &n);
    fclose(input);

    double** matrix = (double**)malloc(n * sizeof(double*));
    for(i = 0; i < n; i++) {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    // Read matrix from mat_{nnnnnn}.in
    char matrix_file_name[100];
    sprintf(matrix_file_name, "inputfiles/mat_%06d.in", n);
    FILE *matrix_file = fopen(matrix_file_name, "r");
    
    if (matrix_file == NULL) {
        perror("Error opening matrix file");
        return 1;
    }

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            fscanf(matrix_file, "%lf,", &matrix[i][j]);
        }
    }
    fclose(matrix_file);

    printf("Matrix file %s read successfully\n", matrix_file_name);

    int num_vecs = 4;

    double** vecs = (double**)malloc(num_vecs * sizeof(double*));
    for(i = 0; i < num_vecs; i++) {
        vecs[i] = (double*)malloc(n * sizeof(double));
    }

    for (i = 0; i < num_vecs; i++) {
        read_vec_file(n, i+1, vecs[i]);
    }

    printf("Vector files read successfully\n\n");

    bool is_eig;
    double eig_val;

    for (i = 0; i < num_vecs; i++) {
        get_eig(n, matrix, vecs[i], &is_eig, &eig_val);
        if (is_eig) {
            printf("vec_%06d_%06d.in : Yes : %lf\n", n, i+1, eig_val);
            append_eig_val(n, i+1, eig_val);
        }
        else {
            printf("vec_%06d_%06d.in : Not an eigenvector\n", n, i+1);
        }
    }

    // Free up memory
    for(i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    for(i = 0; i < num_vecs; i++) {
        free(vecs[i]);
    }
    free(vecs);

    return 0;
}