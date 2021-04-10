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

int recognition_word (char **end, char *final_str, int *j, char id_str[][max], int *id_int, int quantity_ind, int *flag_letter) {
    if (isdigit(**end)) {
        while (**end != 0 && !memchr(const_p, **end, sign) && **end != '(' && **end != ')') {
            if (!isdigit(**end)) {
                return 0;
            }
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
        }
        return 1;
    } else {
        char str_rec[max];
        int i = 0;
        while (**end != 0 && !memchr(const_p, **end, sign) && **end != ')') {
            if (**end == '(') {
                return 0;
            }
            str_rec[i] = **end;
            i++;
            (*end)++;
        }
        str_rec[i] = '\0';
        for (int n = 0; n < quantity_ind; n++) {
                if (!strcmp(id_str[n], str_rec)) {
                    int_in_char(final_str, j, id_int, n);
                    return 1;
                }
            }
        strcat(final_str, str_rec);
        (*j) += i;
        (*flag_letter)++;
        return 1;
    }
}

int recognition_common_statement (char **end, char *final_str, int *j, char id_str[][max], int *id_int, int quantity_ind, int *flag_letter) {
    if (**end == '(') {
        final_str[*j] = **end;
        (*j)++;
        (*end)++;
        int value1 = recognition_common_statement(end, final_str, j, id_str, id_int, quantity_ind, flag_letter);
        if (memchr(const_p, **end, sign)) {
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
        } else {
            return 0;
        }
        int value2 = recognition_common_statement(end, final_str, j, id_str, id_int, quantity_ind, flag_letter);
        if (value1 && value2 && **end == ')'){
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
            return 1;
        } else {
            return 0;
        }
    } else if (!memchr(const_p, **end, sign) && **end != ')' && **end != 0 && **end != '_' && recognition_word(end, final_str, j, id_str, id_int, quantity_ind, flag_letter)) {
        return 1;
    } else {
        return 0;
    }
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

    if (recognition_common_statement(&end, final_str, &j, ind_char, ind_int, quantity_ind, &flag_letter)/*common(&end, final_str, &j, ind_char, ind_int, quantity_ind, flag, &flag_letter)*/) {  //Выводим полученный массив, если мы не подняли флаг на крах рекурсии, и, если кол-во знаков совпадает с кол-вом открывающих скобок.
        final_str[j] = 0;
        printf("%s", final_str);
//        printf("1");

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