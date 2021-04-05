#include <stdio.h>
#include <math.h>
#define max_N 100
#define max_k 64

void quick_sort(int *array, int first, int last, int *address_matrix[], int *k_matrix) {
    if (first < last)
    {
        int left = first;
        int right = last;
        int middle = array[(left + right) / 2];
        while (left <= right) {
            while (array[left] < middle) {
                left++;
            }
            while (array[right] > middle) {
                right--;
            }
            if (left <= right) {
                int x = array[left];
                array[left] = array[right];
                array[right] = x;

                int *add = address_matrix[left];
                address_matrix[left] = address_matrix[right];
                address_matrix[right] = add;

                int k_mat = k_matrix[left];
                k_matrix[left] = k_matrix[right];
                k_matrix[right] =k_mat;

                left++;
                right--;
            }
        }
        quick_sort(array, first, right, address_matrix, k_matrix);
        quick_sort(array, left, last, address_matrix, k_matrix);
    }
    return;
}

void sort_bubble (int size, int *array) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {              // меняем элементы местами, если один больше другого
                int x;
                x = array[j];
                array[j] = array[j + 1];
                array[j + 1] = x;
            }
        }
    }
    return;
}

int det (int matrix[][max_k], int k, int set_Col[], int start_Row, int minor_rank) {                //Рекурсивная функция на нахождениие определиеля
    int x = 0;
    int minus_one_score = 0;
    for (int i = 0; i < k; i++) {
        if (set_Col[i]) {
            if (minor_rank == 1) {
                return matrix[start_Row][i];
            } else {
                start_Row++;
                set_Col[i]--;
                x += pow((-1), minus_one_score)*matrix[start_Row - 1][i]*det(matrix, k, set_Col, start_Row, minor_rank - 1);
                set_Col[i]++;
                minus_one_score++;
                start_Row--;
            }
        }
    }
    return x;
}

int main() {
    int N;                    //Кол-во матриц
    int matrix[max_N][max_k][max_k];        //массив для хранения матриц
    int det_matrix[max_N];
    int *address_matrix[max_N];
    int k_matrix[max_N];                    //Размерность матриц



    int set_Col[max_k];                         //Массив с "пометками", какую строку мы вычеркиваем
    for (int i = 0; i < max_k; i++) {
        set_Col[i] = 1;
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
        address_matrix[i] = *matrix[i];
        det_matrix[i] = det(matrix[i], k_matrix[i], set_Col, 0, k_matrix[i]);
    }


    fclose(input);
    output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Can't open file output.txt");
        return 1;
    }


    quick_sort(det_matrix, 0, N - 1, address_matrix, k_matrix);
    for (int n = 0; n < N; n++) {
//        printf ("%d\n", det_matrix[n]);
        for (int i = 0; i < k_matrix[n]; i++) {                      //начинаем считывать матрицу
            for (int j = 0; j < k_matrix[n]; j++) {
                fprintf(output,"%d ", *(address_matrix[n] + (i * max_k) + j));
            }
            fprintf (output, "\n");
        }
    }

    fclose(output);
    return 0;
}