#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define max 256
#define sign 4
#define const_p "+-/*"

int counting_len_int (int x_int) {
    int len_x = 1;
    while (x_int / 10){
        x_int /= 10;
        len_x++;
    }
    return len_x;
}


void int_in_char (char *final_str, int *j, int *ind_int, char *ind_char, int quantity_ind, char *end) {
    int x = (char *) memchr(ind_char, *end, quantity_ind) - ind_char;          //Ищем номер ячейки, где лежит идекc
    int len_x = counting_len_int(ind_int[x]);
    char y;
    int r = ind_int[x];
    for (int i = len_x; i > 0; i--) {
        y = r / (pow(10, i - 1)) + '0';
        final_str[*j] = y;
        r %= (int) pow(10, i - 1);
        (*j)++;
    }
    return;
}

void common (char **end, char *final_str, int *j, char *ind_char, int *ind_int, int quantity_ind, int *flag, int *score_p, int *score_common) {
    while (**end != ')') {
        if (**end == '(' && *(*end + 1) != 0 && !memchr(const_p, *(*end + 1), sign)) {                                        //Если встречаем открывающую скобку
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
            (*score_common)++;
            common(end, final_str, j, ind_char, ind_int, quantity_ind, flag, score_p, score_common);
        } else if (memchr(const_p, **end, sign) && *(*end + 1) != 0 && *(*end + 1) != ')') {                     //Если встречаем знак
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
            (*score_p)++;
        } else if (memchr(ind_char, **end, quantity_ind) &&
                   ((memchr(const_p, *(*end + 1), sign) && *(*end - 1) == '(') ||
                    (memchr(const_p, *(*end - 1), sign) && *(*end + 1) ==
                                                           ')'))) {                 //Если встречаем индекс
            int_in_char(final_str, j, ind_int, ind_char, quantity_ind, *end);
            (*end)++;
        } else if (!isdigit(**end) && !memchr(const_p, **end, sign) && !memchr(const_p, *(*end + 1), sign) && *(*end + 1) != ')' && **end != '(' ||
                   isdigit(**end) && *(*end + 1) != 0) {              //Если встречаем буквенное выражение или число
            while (!memchr(const_p, **end, sign) && **end !=
                                                    ')' && **end != '(') {      //То идем по строке пока не встретим символ (нужно с последними скобками все отладить)
                final_str[*j] = **end;
                (*end)++;
                (*j)++;
            }
        } else {
            (*flag)++;
            return;
        }
    }
    final_str[*j] = **end;
    (*j)++;
    (*end)++;
    return;
}


int main() {
    int quantity_ind;
    scanf("%d\n", &quantity_ind);
    char ind_char[max];
    int ind_int[max];
    for (int i = 0; i < quantity_ind; i++) {
        scanf("%c %d\n", &ind_char[i], &ind_int[i]);
    }
    ind_char[quantity_ind] = 0;
    char str[max];
    char final_str[max];
    scanf("%s", str);
    int j = 0;
    char *end = str;
    int flag = 0;

                 //переменные - счетчики, на правильность выражения
    int score_p = 0;
    int score_common = 0;

    if (*end == '(' && *(end + 1) != 0 && !memchr(const_p, *(end + 1), sign)) {
        final_str[j] = *end;
        j++;
        end++;
        score_common++;
        common(&end, final_str, &j, /*&n,*/ ind_char,  ind_int, quantity_ind, &flag, /*&score_unique_ind,*/ &score_p, &score_common);
    } else if (memchr(ind_char, *end, quantity_ind)) {                 //Если встречаем индекс
        int_in_char(final_str, &j, ind_int, ind_char, quantity_ind, end);
        end++;
    } else if (isdigit(*end)) {              //Если встречаем число
        while (*end != 0) {
            if (*end == '(' || *end == ')' || !isdigit(*end)) {
                flag++;
            }
            final_str[j] = *end;
            end++;
            j++;
        }
    } else {
        flag++;
    }

    if (!flag && score_p == score_common) {
    final_str[j] = 0;
    printf("%s\n", final_str);
    } else {
        printf ("INCORRECT");
    }
    return 0;
}