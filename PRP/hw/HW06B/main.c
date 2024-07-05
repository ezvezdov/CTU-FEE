#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MATRIX_COUNT 200
#define START_M 1
#define START_N 1
#define SIZE 100

void allocation_memory_error() {
  fprintf(stderr, "Error: Memory allocation error!\n");
  exit(1);
}

void exit_with_error_100() {
  fprintf(stderr, "Error: Chybny vstup!\n");
  exit(100);
}

/*allocating dynamic memory for matrix*/
int **allocating_matrix(int **matrix, int m, int n) {
  matrix = (int **)malloc(m * sizeof(int *));
  if (matrix == NULL) {
    allocation_memory_error();
  }
  for (int i = 0; i < m; i++) {
    matrix[i] = (int *)malloc(n * sizeof(int));
    if (matrix[i] == NULL) {
      allocation_memory_error();
    }
  }
  return matrix;
}

int** matrixes_realloc(int **matrix, int capacity_m, int capacity_n,int flag_m,int flag_n) {
  int **new_matrix = NULL;
  new_matrix = malloc(capacity_m * sizeof(int*));

  for (int i = 0; i < capacity_m; i++) {
    new_matrix[i] = malloc(capacity_n * sizeof(int));
    for(int j = 0; i != capacity_m-flag_m && j < capacity_n-flag_n; j++){
        new_matrix[i][j] = matrix[i][j];
    }
  }
  return new_matrix;
}

/*reallocating memmrory for input string*/
char *reallocating_memmory(char *str, int capacity) {
  char *new_string = NULL;
  new_string = realloc(str, capacity);
  if (new_string == NULL) {
    allocation_memory_error();
  }
  str = new_string;
  return str;
}

/*free allocated dynamic memory for matrix*/
void free_allocated_matrix(int **matrix, int m) {
  for (int i = 0; i < m; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

/*input matrix*/
int** read_matrix(int *m, int *n, int *matrix_index,
                int *M_CAPACITY) {
  char matrix_name = getchar();
  if (matrix_name == '\n') {
    return NULL;
  }
  *matrix_index = (matrix_name);
  char tmp_char = getchar();

  if (tmp_char != '=') {
    exit_with_error_100();
  }
  tmp_char = getchar();
  if (tmp_char != '[') {
    exit_with_error_100();
  }

  int capacity_m = START_M;
  int capacity_n = START_N;
  int i = 0;
  int j = 0;

  int **matrix = NULL;
  matrix = allocating_matrix(matrix, START_M, START_N);
  int** tmp = NULL;

  while (1) {
    
    if(i >= capacity_m || j >= capacity_n){
      tmp = matrixes_realloc(matrix,capacity_m + (i >= capacity_m), capacity_n + (j >= capacity_n),(i >= capacity_m),(j >= capacity_n));
      free_allocated_matrix(matrix,capacity_m);
      matrix = tmp;
      tmp = NULL;
      capacity_m += (i >= capacity_m) ;
      capacity_n += (j >= capacity_n);
    }    

    int ret = scanf("%d", &matrix[i][j]);
    if (ret == EOF || ret != 1) {
      exit_with_error_100();
    }
    tmp_char = getchar();
    if (tmp_char == ';') {
      tmp_char = getchar();
      i++;
      j = 0;
      continue;
    }
    if (tmp_char == ']') {
      tmp_char = getchar();
      *m = i + 1;
      *n = j + 1;
      break;
    }
    j++;
  }
  *M_CAPACITY = capacity_m;
  return matrix;
}

/*get string (all characters on line) from input*/
char *getline(int *len) {
  char in_char;
  int capacity = SIZE;
  char *str = NULL;
  str = malloc(capacity);
  if (str == NULL) {
    allocation_memory_error();
  }

  while ((in_char = getchar()) != EOF && in_char != '\n') {
    if (in_char < 'A' || in_char > 'Z') {
      if (in_char != '+' && in_char != '-' && in_char != '*') {
        fprintf(stderr, "Error: Chybny vstup!\n");
        exit(100);
      }
    }
    str[*len] = in_char;
    (*len)++;
    if (*len == capacity) {
      capacity *= 2;
      str = reallocating_memmory(str, capacity);
    }
    str[*len] = '\0';
  }

  return str;
}

/*Copy src_matrix data to matrix*/
void make_matrix_copy(int **matrix, int **src_matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = src_matrix[i][j];
    }
  }
}

int is_equal_matrix(int** matrix1, int m1, int n1, int** matrix2, int m2, int n2){
  if(m1 != m2 || n1 != n2){
    return 0;
  }
  for(int i = 0; i < m1; i++){
    for(int j = 0; j < n1; j++){
      if(matrix1[i][j] != matrix2[i][j]){
        return 0;
      }
    }
  }
  return 1;
}

/*Output matrix.*/
void print_matrix(int **matrix, int m, int n) {
  printf("[");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d", matrix[i][j]);
      if (j + 1 < n) {
        printf(" ");
      }
    }
    if (i + 1 < m) {
      printf("; ");
    }
  }
  printf("]\n");
}

void sum_of_matrixes(int **first_matrix, int **second_matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      first_matrix[i][j] = first_matrix[i][j] + second_matrix[i][j];
    }
  }
}

void difference_of_matrixes(int **first_matrix, int **second_matrix, int m,
                            int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      first_matrix[i][j] = first_matrix[i][j] - second_matrix[i][j];
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
  int **matrixes_array[MAX_MATRIX_COUNT] = {0};
  int sizes_array[MAX_MATRIX_COUNT][2] = {{0}};
  int capacity_array[MAX_MATRIX_COUNT] = {0};

  int matrixes_names[MAX_MATRIX_COUNT] = {0};
  int names_index = 0;

  int ret = 0;
  int matrix_index = 0;

  do {
    int m = 0, n = 0;
    int m_capacity = START_M;

    int **matrix = NULL;
    matrix = read_matrix(&m, &n, &matrix_index, &m_capacity);
    if (matrix == NULL) {
      break;
    }
    
    

    matrixes_array[matrix_index] = matrix;
    sizes_array[matrix_index][0] = m;
    sizes_array[matrix_index][1] = n;
    capacity_array[matrix_index] = m_capacity;

    matrixes_names[names_index] = matrix_index;
    names_index++;
  } while (ret != EOF);

  int operations_size = 0;
  char *operations = getline(&operations_size);
  char *operations_update = calloc('0', operations_size);
  int operations_update_index = 0;
  int current_new_matrix_index = 'a';

  for (int i = 0; i < operations_size; i++) {
    if (operations[i] != '*') {
      operations_update[operations_update_index] = operations[i];
    } else {
      int matrix_1 = (int)operations_update[operations_update_index - 1],
          matrix_2 = (int)operations[i + 1];

      if (sizes_array[matrix_1][1] != sizes_array[matrix_2][0]) {
        exit_with_error_100();
      }

      int result_m = sizes_array[matrix_1][0],
          result_n = sizes_array[matrix_2][1];
      int **result_matrix = NULL;
      result_matrix = allocating_matrix(result_matrix, result_m, result_n);
      multipy_of_matrixes(matrixes_array[matrix_1], matrixes_array[matrix_2],
                          result_matrix, sizes_array[matrix_1][0],
                          sizes_array[matrix_2][1], sizes_array[matrix_1][1]);

      if (matrix_1 >= 'a' || matrix_1 < 'A') {
        current_new_matrix_index--;
        names_index--;
        free_allocated_matrix(matrixes_array[matrix_1],
                              sizes_array[matrix_1][0]);
      }

      matrixes_array[current_new_matrix_index] = result_matrix;
      sizes_array[current_new_matrix_index][0] = result_m;
      sizes_array[current_new_matrix_index][1] = result_n;
      capacity_array[current_new_matrix_index] = result_m;

      matrixes_names[names_index] = current_new_matrix_index;

      operations_update_index--;
      operations_update[operations_update_index] = current_new_matrix_index;

      names_index++;
      current_new_matrix_index++;
      i++;

      if (current_new_matrix_index == '~') {
        current_new_matrix_index = ' ';
      }
      if (current_new_matrix_index == '*') {
        current_new_matrix_index = 44;
      }
    }
    operations_update_index++;
  }
  free(operations);
  operations = operations_update;
  operations_size = operations_update_index;

  int result_m = sizes_array[(int)operations[0]][0];
  int result_n = sizes_array[(int)operations[0]][1];
  int **result_matrix = NULL;
  result_matrix = allocating_matrix(result_matrix, result_m, result_n);
  make_matrix_copy(result_matrix, matrixes_array[(int)operations[0]], result_m,
                   result_n);

  for (int i = 1; i < operations_size; i++) {
    if (operations[i] == '+' || operations[i] == '-') {
      if (result_m != sizes_array[(int)operations[i + 1]][0] ||
          result_n != sizes_array[(int)operations[i + 1]][1]) {
        exit_with_error_100();
      }
    }

    if (operations[i] == '+') {
      sum_of_matrixes(result_matrix, matrixes_array[(int)operations[i + 1]],
                      result_m, result_n);
    } else if (operations[i] == '-') {
      difference_of_matrixes(result_matrix,
                             matrixes_array[(int)operations[i + 1]], result_m,
                             result_n);
    }
  }
  print_matrix(result_matrix, result_m, result_n);

  for (int i = 0; i < names_index; i++) {
    free_allocated_matrix(matrixes_array[matrixes_names[i]],
                          capacity_array[matrixes_names[i]]);
  }
  free_allocated_matrix(result_matrix, result_m);
  free(operations);

  return 0;
}
