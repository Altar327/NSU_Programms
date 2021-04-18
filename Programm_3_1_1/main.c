#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define max_N 50
#define max_k 64
#define num_loops 100

void swap (int *a, int *b) {
    int x = *a;
    *a = *b;
    *b = x;
    return;
}

void quick_sort(int *array, int first, int last, int *address_matrix) {
    if (first < last)
    {
        int left = first;
        int right = last;
        int middle = rand() % last;
        while (left <= right) {
            while (array[left] < middle) {
                left++;
            }
            while (array[right] > middle) {
                right--;
            }
            if (left <= right) {
                swap (&array[left], &array[right]);
                swap (&address_matrix[left], &address_matrix[right]);
                left++;
                right--;
            }
        }
        quick_sort(array, first, right, address_matrix);
        quick_sort(array, left, last, address_matrix);
    }
    return;
}

void sort_bubble (int size, int *array) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {              // меняем элементы местами, если один больше другого
                swap (&array[j], &array[j + 1]);
            }
        }
    }
    return;
}

int det (int matrix[][max_k], int k, int set_col[], int start_row, int minor_rank) {                //Рекурсивная функция на нахождениие определиеля
    int x = 0;
    int minus_one_score = 0;
    for (int i = 0; i < k; i++) {
        if (set_col[i]) {
            if (minor_rank == 1) {
                return matrix[start_row][i];
            } else {
                start_row++;
                set_col[i]--;
                x += pow((-1),minus_one_score) * matrix[start_row - 1][i]*det(matrix, k, set_col, start_row, minor_rank - 1);
                set_col[i]++;
                minus_one_score++;
                start_row--;
            }
        }
    }
    return x;
}

void randomiz () {
    FILE *rand_input;
    rand_input = fopen("input.txt", "w");
    int r_N = 10;
    fprintf (rand_input, "%d\n", r_N);
    for (int n = 0; n < r_N; n++) {
        int k_matrix = rand() % 10;
        fprintf (rand_input, "%d\n", k_matrix);
        for (int i = 0; i < k_matrix; i++) {                      //начинаем считывать матрицу
            for (int j = 0; j < k_matrix; j++) {
                fprintf(rand_input, "%d ", rand() % 10);
            }
            fprintf(rand_input, "\n");
        }
    }
    fclose(rand_input);
    return;
}

int main() {
    srand(time(NULL));

    randomiz();

    clock_t start, finish;
    double sum_time = 0;

    FILE *time_output;
    time_output = fopen("time_output.txt", "w");

    for (int loops = 0; loops < num_loops; loops++) {
        int N;                    //Кол-во матриц
        int matrix[max_N][max_k][max_k];        //массив для хранения матриц
//        int ***matrix = (int ***) malloc(max_N * sizeof(int **));           //массив для хранения матриц
//        for (int i = 0; i < max_k; i++) {
//            matrix[i] = (int **) malloc(max_k * sizeof(int *));
//            for (int j = 0; j < max_k; j++) {
//                matrix[i][j] = (int *) malloc(max_k * sizeof(int));
//            }
//        }

//         int* A = (int*)malloc(M * N * O * sizeof(int));
        int *det_matrix = malloc(N * sizeof(int));
        int *address_matrix = malloc(N * sizeof(int));
        int *k_matrix = malloc(N * sizeof(int));             //Размерность матриц
//         int det_matrix[max_N];
//         int address_matrix[max_N];
//         int k_matrix[max_N];                    //Размерность матриц
        start = clock();

        int set_col[max_k];                         //Массив с "пометками", какую строку мы вычеркиваем
        for (int i = 0; i < max_k; i++) {
            set_col[i] = 1;
        }

        FILE *input, *output;
        input = fopen("input.txt", "r");
        if (input == NULL) {
            printf("Can't open file input.txt");
            return 1;
        }
        fscanf(input, "%d", &N);

        for (int n = 0; n < N; n++) {                   //Cчитываем матрицы в n ячеек 3-х мерной матрицы
            fscanf(input, "%d", &k_matrix[n]);
            for (int i = 0; i < k_matrix[n]; i++) {                      //начинаем считывать матрицу
                for (int j = 0; j < k_matrix[n]; j++) {
                    fscanf(input, "%d", &matrix[n][i][j]);
                }
            }
        }

        for (int i = 0; i < N; i++) {
            address_matrix[i] = i;
            det_matrix[i] = det(matrix[i], k_matrix[i], set_col, 0, k_matrix[i]);
        }


        fclose(input);
        output = fopen("output.txt", "w");
        if (output == NULL) {
            printf("Can't open file output.txt");
            return 1;
        }

        quick_sort(det_matrix, 0, N - 1, address_matrix);
        for (int n = 0; n < N; n++) {
            for (int i = 0; i < k_matrix[address_matrix[n]]; i++) {                      //начинаем считывать матрицу
                for (int j = 0; j < k_matrix[address_matrix[n]]; j++) {
                    fprintf(output, "%d ", matrix[address_matrix[n]][i][j]);
                }
                fprintf(output, "\n");
            }
        }

        fclose(output);
//        Sleep(1);
        finish = clock();
        sum_time += (finish - start) / CLOCKS_PER_SEC;
        fprintf(time_output, "%2.3f\n", (double) (finish - start) / CLOCKS_PER_SEC);

        free(det_matrix);
        free(address_matrix);
        free(k_matrix);
//        for (int i = 0; i < max_N; i++) {
//            for (int j = 0; j < max_k; j++) {
//                free(matrix[i][j]);
//            }
//            free(matrix[i]);
//        }
//        free(matrix);
    }
     fclose(time_output);

    return 0;
}