#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MATRIX_COUNT 100

void exit_with_error_100() {
  fprintf(stderr, "Error: Chybny vstup!\n");
  exit(100);
}

/*allocating dynamic memory for matrix*/
int **allocating_matrix(int **matrix, int m, int n) {
  matrix = (int **)malloc(m * sizeof(int *));
  if (matrix == NULL) {
    exit_with_error_100();
  }
  for (int i = 0; i < m; i++) {
    matrix[i] = (int *)malloc(n * sizeof(int));
    if (matrix[i] == NULL) {
      exit_with_error_100();
    }
  }
  return matrix;
}

/*free allocated dynamic memory for matrix*/
void free_allocated_matrix(int **matrix, int m) {
  for (int i = 0; i < m; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

void read_matrix(int **matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      int ret = scanf("%d ", &matrix[i][j]);
      if (ret == EOF || ret != 1) {
        exit_with_error_100();
      }
    }
  }
}

void read_sizes(int *m, int *n) {
  int ret = scanf("%d %d", m, n);
  if (ret == EOF || ret != 2) {
    exit_with_error_100();
  }
}

void print_matrix(int **matrix, int m, int n) {
  printf("%d %d\n", m, n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d", matrix[i][j]);
      if (j + 1 < n) {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void sum_of_matrixes(int **first_matrix, int **second_matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      second_matrix[i][j] = first_matrix[i][j] + second_matrix[i][j];
    }
  }
}

void differemce_of_matrixes(int **first_matrix, int **second_matrix, int m,
                            int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      second_matrix[i][j] = first_matrix[i][j] - second_matrix[i][j];
    }
  }
}

void multipy_of_matrixes(int **first_matrix, int **second_matrix,
                         int **result_matrix, int first_m, int second_n,
                         int same_n) {
  for (int i = 0; i < first_m; i++) {
    for (int j = 0; j < second_n; j++) {
      int multiply = 0;
      for (int k = 0; k < same_n; k++) {
        multiply += first_matrix[i][k] * second_matrix[k][j];
      }
      result_matrix[i][j] = multiply;
    }
  }
}

int main(int argc, char *argv[]) {
  char operations[MAX_MATRIX_COUNT] = {0};
  int operation_size = 0;

  int matrixes_count = 0;
  int **matrixes_array[MAX_MATRIX_COUNT] = {0};
  int sizes_array[MAX_MATRIX_COUNT][2] = {{0}};

  do {
    if (matrixes_count != 0 && operations[operation_size] != '*' &&
        operations[operation_size] != '+' &&
        operations[operation_size] != '-') {
      exit_with_error_100();
    }
    if (matrixes_count != 0) {
      operation_size++;
    }

    int m, n;
    read_sizes(&m, &n);
    sizes_array[matrixes_count][0] = m;
    sizes_array[matrixes_count][1] = n;
    matrixes_array[matrixes_count] =
        allocating_matrix(matrixes_array[matrixes_count], m, n);
    read_matrix(matrixes_array[matrixes_count], m, n);

    if (operation_size != 0 && operations[operation_size - 1] == '*') {
      if (sizes_array[matrixes_count - 1][1] !=
          sizes_array[matrixes_count][0]) {
        exit_with_error_100();
      }

      int result_m = sizes_array[matrixes_count - 1][0],
          result_n = sizes_array[matrixes_count][1];
      int **result_matrix = NULL;
      result_matrix =
          (int **)allocating_matrix(result_matrix, result_m, result_n);

      multipy_of_matrixes(
          matrixes_array[matrixes_count - 1], matrixes_array[matrixes_count],
          result_matrix, sizes_array[matrixes_count - 1][0],
          sizes_array[matrixes_count][1], sizes_array[matrixes_count - 1][1]);

      free_allocated_matrix(matrixes_array[matrixes_count - 1],
                            sizes_array[matrixes_count - 1][0]);
      free_allocated_matrix(matrixes_array[matrixes_count],
                            sizes_array[matrixes_count][0]);
      matrixes_array[matrixes_count - 1] = result_matrix;
      sizes_array[matrixes_count - 1][0] = result_m;
      sizes_array[matrixes_count - 1][1] = result_n;

      operation_size--;
      matrixes_count--;
    }

    matrixes_count++;
  } while (scanf("%c", &operations[operation_size]) != EOF &&
           operations[operation_size] != '\n');

  for (int i = 0; i < operation_size; i++) {
    if (sizes_array[i][0] != sizes_array[i + 1][0] ||
        sizes_array[i][1] != sizes_array[i + 1][1]) {
      exit_with_error_100();
    }

    if (operations[i] == '+') {
      sum_of_matrixes(matrixes_array[i], matrixes_array[i + 1],
                      sizes_array[i][0], sizes_array[i][1]);
    } else if (operations[i] == '-') {
      differemce_of_matrixes(matrixes_array[i], matrixes_array[i + 1],
                             sizes_array[i][0], sizes_array[i][1]);
    }

    free_allocated_matrix(matrixes_array[i], sizes_array[i][0]);
  }

  print_matrix(matrixes_array[matrixes_count - 1],
               sizes_array[matrixes_count - 1][0],
               sizes_array[matrixes_count - 1][1]);

  free_allocated_matrix(matrixes_array[matrixes_count - 1],
                        sizes_array[matrixes_count - 1][0]);

  return 0;
}
