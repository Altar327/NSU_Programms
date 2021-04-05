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

void int_in_char (char *final_str, int *j, int *ind_int, char *end, int x) {
//    int x = (char *) memchr(ind_char, *end, quantity_ind) - ind_char;                       //Ищем номер ячейки, где лежит индекc
    int len_x = counting_len_int(ind_int[x]);
    int r = ind_int[x];
    for (int i = len_x; i > 0; i--) {                           //Записываем в строку по порядку остатки
        final_str[*j] = r / (pow(10, i - 1)) + '0';
        r %= (int) pow(10, i - 1);
        (*j)++;
    }
    return;
}

void recognition (char *final_str, int *j, char *ind_str[], int *ind_int, int quantity_ind, char *end_rec, *flag) {
    if (isdigit(*end_rec)) {
        while (*end_rec != 0) {
            if (!isdigit(*end_rec)) {
                (*flag)++;
                return;
            }
            final_str[*j] = *end_rec;
            (*j)++;
            (*end_rec)++;
        }
    } else {
        while (){

        }

    }
}

void common (char **end, char *final_str, int *j, char *ind_str[], int *ind_int, int quantity_ind, int *flag,
       int *flag_letter) {
    while (**end != ')' && **end != 0) {
        int x = 0;
        char str_for_recognition[max];
        if (**end == '(') {                     //Вызываем рекурсию если увидели вложенный блок
            final_str[*j] = **end;
            (*j)++;
            (*end)++;
            common(end, final_str, j, ind_str, ind_int, quantity_ind, flag, flag_letter);
        } else if (!memchr(const_p, **end, sign)) {
            int i = 0;
            while (!memchr(const_p, **end, sign)) {
                str_for_recognition[i] = **end;
                i++;
                (*end)++;
                if (**end == ')' || **end == '(' || **end == 0) {
                    (*flag)++;
                    return;
                }
            }
            str_for_recognition[i] = '\0';
            char *end_rec = str_for_recognition;
            recognition(final_str, *j, ind_str[], *ind_int, quantity_ind, end_rec, flag);
            if (memchr(const_p, **end, sign)) {
                if (x != 0) {
                    (*flag)++;
                    return;
                }
                final_str[*j] = **end;
                (*j)++;
                (*end)++;
                x++;
            }

        } else {                    //Если все сломалось
            (*flag)++;
            return;
        }
    }
    final_str[*j] = **end;
    (*j)++;
    (*end)++;
    return;
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

    //переменные - счетчики, на правильность выражения
    int score_p = 0;
    int score_common = 0;

    int flag_letter = 0;

    if (*end == '(' && *(end + 1) != 0 && !memchr(const_p, *(end + 1), sign)) { //Если встречаем конструкцию (... + ...)
        final_str[j] = *end;
        j++;
        end++;
        score_common++;
        common(&end, final_str, &j, ind_char, ind_int, quantity_ind, &flag, &score_p,
               &score_common, &flag_letter);                                                                                          //Вызываем рекурсию
    } else if (memchr(ind_char, *end, quantity_ind)) {                 //Если встречаем индекс заменяем его и выводим
        int_in_char(final_str, &j, ind_int, ind_char, quantity_ind, end);
        end++;
    } else if (isdigit(*end)) {              //Если встречаем число
        while (*end != 0) {                     //то идем до конца строки
            if (*end == '(' || *end == ')' || !isdigit(*end) || !memchr(const_p, *end, sign)) { //проверяем что ввели только число
                flag++;
            }
            final_str[j] = *end;
            end++;
            j++;
        }
    } else {
        flag++;
    }

    if (!flag) {  //Выводим полученный массив, если мы не подняли флаг на крах рекурсии, и, если кол-во знаков совпадает с кол-вом открывающих скобок.
        final_str[j] = 0;
        printf("%s", final_str);

        //Вывод значения выражения

        if (!flag_letter) {
            char *end_counting = final_str;
            end_counting++;
            printf(" %c %d", 26, counting_statement(&end_counting));                //Выпендрежный вывод
        }

    } else {
        printf("INCORRECT");
    }
    return 0;
}