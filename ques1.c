#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void print_to_file(int n, double **A, int format_flag) {
    char filename[100];
    if (format_flag == 0) {
        sprintf(filename, "array_%06d_asc.out", n);
        FILE *output = fopen(filename, "w");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(output, "%.15lf ", A[i][j]);
            }
            fprintf(output, "\n");
        }
        fclose(output);
    }
    else if (format_flag == 1) {
        sprintf(filename, "array_%06d_bin.out", n);
        FILE *output = fopen(filename, "wb");
        for (int i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, output);
        }
        fclose(output);
    }
}

int main() {
    int n, i, j;

    // Read n from "input.in"
    FILE *input = fopen("input_ques1.in", "r");
    fscanf(input, "%d", &n);
    fclose(input);

    // Dynamic allocation of A size n*n
    double **A = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
    }

    // Fill A with i+j
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }

    // Print A to file in ascii format
    print_to_file(n, A, 0);

    // Print A to file in binary format
    print_to_file(n, A, 1);

    // Free up memory
    for (i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
