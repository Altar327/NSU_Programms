#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define max 256
#define sign 4
#define const_p "+-/*"

int counting_len_int(int x_int) {               //Функция для вычисления "длины" int
    int len_x = 1;
    while (x_int / 10) {
        x_int /= 10;
        len_x++;
    }
    return len_x;
}

void int_in_char (char *final_str, int *j, int *id_int, int x) {
//    int x = (char *) memchr(ind_char, *end, quantity_ind) - ind_char;                       //Ищем номер ячейки, где лежит индекc
    int len_x = counting_len_int(id_int[x]);
    int r = id_int[x];
    for (int i = len_x; i > 0; i--) {                           //Записываем в строку по порядку остатки
        final_str[*j] = r / (pow(10, i - 1)) + '0';
        r %= (int) pow(10, i - 1);
        (*j)++;
    }
    return;
}

int recognition (char *end_rec, int *flag_letter) {
    if (isdigit(*end_rec)) {
        while (*end_rec != 0 && !memchr(const_p, *end_rec, sign) && *end_rec != '(' && *end_rec != ')') {
            if (!isdigit(*end_rec)) {
                return 0;
            }
            end_rec++;
        }
        return 1;
    } else {
        while (*end_rec != 0 && !memchr(const_p, *end_rec, sign) && *end_rec != '(') {
            if (*end_rec == '(') {
                return 0;
            }
            end_rec++;
        }
        return 2;
    }
}

void record (char **end, char *final_str, int *j, char id_str[][max], int *id_int, int quantity_ind, int *flag_letter) {
    int y = recognition(*end, flag_letter);
    switch (y) {
        case 0: {
            return;
        }
        case 1: {
            while (!memchr(const_p, **end, sign) && **end != 0 && **end != ')') {
                final_str[*j] = **end;
                (*j)++;
                (*end)++;
            }
            return;
        }
        case 2: {
            char str_rec[max];
            int len_str_rec = 0;
            while (!memchr(const_p, **end, sign) && **end != ')' && **end != 0) {
                str_rec[len_str_rec] = **end;
                len_str_rec++;
                (*end)++;
            }
            str_rec[len_str_rec] = '\0';
            for (int i = 0; i < quantity_ind; i++) {
                if (!strcmp(id_str[i], str_rec)) {
                    int_in_char(final_str, j, id_int, i);
                    return;
                }
            }
            strcat(final_str, str_rec);
            (*j) += len_str_rec;
            (*flag_letter)++;
            return;
        }
    }
}

int common (char **end, char *final_str, int *j, char id_str[][max], int *id_int, int quantity_ind, int flag, int *flag_letter) {
    int x = 0;
    if (**end == '(') {                     //Вызываем рекурсию если увидели вложенный блок
        final_str[*j] = **end;
        (*j)++;
        (*end)++;
        flag++;
        x = common(end, final_str, j, id_str, id_int, quantity_ind, flag, flag_letter);
    } else if (!memchr(const_p, **end, sign) && **end != ')' && **end != 0 && **end != '_') {           //Проверяем следующий символ после '(' или первый символ
        record (end, final_str, j, id_str, id_int, quantity_ind, flag_letter);
        if (memchr(const_p, **end, sign) && flag) {                 //Смотрим, что это составное выражение, а не просто число или id
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
            int y = 0;
            if (**end == '(') {                     //проверяем выражение вида (...+(...))
                final_str[*j] = **end;
                (*j)++;
                (*end)++;
                y = common(end, final_str, j, id_str, id_int, quantity_ind, flag, flag_letter);
            } else if (!memchr(const_p, **end, sign) && **end != ')' && **end != 0 && **end != '_') {
                record(end, final_str, j, id_str, id_int, quantity_ind, flag_letter);
            } else {
                return 0;
            }
            if (**end == ')' && y) {
                final_str[*j] = **end;
                (*j)++;
                (*end)++;
                if (memchr(const_p, **end, sign)) {
                    return common(end, final_str, j, id_str, id_int, quantity_ind, flag, flag_letter);
                }
                return 1;
            }
        } else if (**end == 0 && !flag) {              //Выход из функции, если распознали только цифру или id
            return 1;
        } else if (**end == ')' && flag) {
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    if (**end == ')'  && x) {
        final_str[*j] = **end;
        (*j)++;
        (*end)++;
        return 1;
    }
    return 1;
}


int counting_elementary_statement (char sign_p, int value1, int value2) {                 //Функция считающая значения блока (... + ...)
    if (sign_p == '+') {
        return (value1 + value2);
    } else if (sign_p == '-') {
        return (value1 - value2);
    } else if (sign_p == '*') {
        return (value1 * value2);
    } else if (sign_p == '/') {
        return (value1/value2);
    }
}

int char_in_int (char *count) {                     //функция перевода char в int
    int x = 0;
    int count_len = strlen(count);
    for (int i = 0; i < count_len; i++) {                   //записываем по порядку единицы, десятки и т.д.
        x += (count[count_len - i - 1] - '0')*pow(10, i);
    }
    return x;
}

int counting_statement(char **end_counting) {               //Фукция считающея значение всего выражения
    int value1, value2 = 0;
    char sign_p;                //Знак в рассматриваемом блоке
    char count[max];                //массив для перевода char в int

    while (**end_counting != ')') {
        if (**end_counting == '(') {                    //вызываем рекурсию если увидели вложенный блок
            if (*(*end_counting - 1) == '(') {
                (*end_counting)++;
                value1 = counting_statement(end_counting);
            } else {
                (*end_counting)++;
                value2 = counting_statement(end_counting);
            }
        } else if (isdigit(**end_counting)) {
            int i = 0;
            while (!memchr(const_p, **end_counting, sign) && **end_counting != ')') {  //Записываем число в массив для дальнейшей обработки
                count[i] = **end_counting;
                i++;
                (*end_counting)++;
            }
            count[i] = '\0';                        //Обозначаем конец строки
            if (**end_counting == ')') {
                value2 = char_in_int(count);
            } else {
                value1 = char_in_int(count);
            }
        } else if (memchr(const_p, **end_counting, sign)) {
            sign_p = **end_counting;
            (*end_counting)++;
        }
    }
    return (counting_elementary_statement (sign_p, value1, value2));                //Возвращаем значение рассматриваемого блока
}

int main() {
    int quantity_ind;
    scanf("%d\n", &quantity_ind);

    char ind_char[max][max];
    int ind_int[max];
    for (int i = 0; i < quantity_ind; i++) {
        scanf("%s %d\n", &ind_char[i], &ind_int[i]);
    }

    char str[max];

    char final_str[max];
    scanf("%s", str);
    int j = 0;
    char *end = str;
    int flag = 0;

    int flag_letter = 0;

    if (common(&end, final_str, &j, ind_char, ind_int, quantity_ind, flag, &flag_letter)) {  //Выводим полученный массив, если мы не подняли флаг на крах рекурсии, и, если кол-во знаков совпадает с кол-вом открывающих скобок.
        final_str[j] = 0;
        printf("%s", final_str);

        //Вывод значения выражения

        char *end_counting = final_str;
        if (!flag_letter && *end_counting == '(') {
            end_counting++;
            printf(" %c %d", 26, counting_statement(&end_counting));                //Выпендрежный вывод
        }

    } else {
        printf("INCORRECT");
    }
    return 0;
}