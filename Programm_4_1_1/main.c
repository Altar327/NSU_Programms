#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define amount_marks 10
#define max 100
#define epsilon 0.000001

typedef struct student_ {
    int name;
    double marks[amount_marks];
    double middle_value;
} student;

typedef struct tree_ {
    int height;
    double value;
    student* students;
    int quantity_students;
    struct tree_ *left_child;
    struct tree_ *right_child;
} tree;

int equally_double (double a, double b) {
    if (fabs(a - b) < epsilon) {
        return 1;
    } else {
        return 0;
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

int ask_height (tree *root) {
    if (root == NULL) {
        return 0;
    } else {
        return root->height;
    }
}

int ask_balance (tree *root) {
    return ask_height(root->right_child) - ask_height(root->left_child);
}

void fix_height (tree *root) {
    int left_height = ask_height(root->left_child);
    int right_height = ask_height(root->right_child);
    if (left_height > right_height) {
        root->height = left_height + 1;
    } else {
        root->height = right_height + 1;
    }
}

tree* rotate_right(tree *root) {                // правый поворот
    tree* q = root->left_child;
    root->left_child = q->right_child;
    q->right_child = root;
    fix_height(root);
    fix_height(q);
    return q;
}

tree* rotate_left(tree *root) {                    // левый поворот
    tree* p = root->right_child;
    root->right_child = p->left_child;
    p->left_child = root;
    fix_height(root);
    fix_height(p);
    return p;
}

tree* balance_tree(tree *root) {                                   // балансировка узла p
    fix_height(root);
    if (ask_balance(root) == 2 ) {
        if (ask_balance(root->right_child) < 0) {
            root->right_child = rotate_right(root->right_child);
        }
        return rotate_left(root);
    }
    if (ask_balance(root) == -2) {
        if (ask_balance(root->left_child) > 0) {
            root->left_child = rotate_left(root->left_child);
        }
        return rotate_right(root);
    }
    return root;                   //если балансировка не нужна
}

tree* insert(tree *root, student stud) {
    if (root == NULL) {                            //Если на вход получили пустой нод
        root = (tree*)malloc(sizeof(tree));
        if (root == NULL) {               //Если система не дала памяти под массив, выходим
            printf("No memory");
            return NULL;
        }
        root->value = stud.middle_value;
        root->students = (student*)malloc(sizeof(student));
        if (root->students == NULL) {               //Если система не дала памяти под массив, выходим
            printf("No memory");
            return NULL;
        }
        *(root->students) = stud;
        root->quantity_students = 1;
        root->left_child = NULL;
        root->right_child = NULL;
        root->height = 1;
        return root;
    } else if (stud.middle_value > root->value) {               //Если нод не пустой, то начинаем искать куда поставить stud
        root->right_child = insert(root->right_child, stud);
    } else if (equally_double(root->value, stud.middle_value)) {
        root->quantity_students++;
        root->students = (student*)realloc(root->students, sizeof(student)*root->quantity_students);            //Реалоком сокращаем размер массива студентов, перевыделяя память меньшего размера
        if (root->students == NULL) {               //Если система не дала памяти под массив, выходим
            printf("No memory");
            return NULL;
        }
        root->students[root->quantity_students - 1] = stud;
        return root;
    } else {
        root->left_child = insert(root->left_child, stud);
    }
    return balance_tree(root);
}

tree* find_min(tree *root) {                   // поиск узла с минимальным ключом в дереве
    if (root->left_child) {
        return find_min(root->left_child);
    } else {
        return root;
    }
}

tree* remove_min(tree *root) {                     // удаление узла с минимальным ключом из дерева p
    if(root->left_child == 0 ) {
        return root->right_child;
    }
    root->left_child = remove_min(root->left_child);
    return balance_tree(root);
}

tree* remove_node (tree *root, double k) {                // удаление ключа k из дерева p
    if (!root) {
        return 0;
    }
    if (k < root->value) {
        root->left_child = remove_node(root->left_child, k);
    } else if (k > root->value) {
        root->right_child = remove_node(root->right_child, k);
    } else {                      //  k == p->key
        tree* q = root->left_child;
        tree* r = root->right_child;

        free(root->students);
        free(root);

        if(!r) {
            return q;
        }
        tree* min = find_min(r);
        min->right_child = remove_min(r);
        min->left_child = q;
        return balance_tree(min);
    }
    return balance_tree(root);
}

tree* change (tree *root, int name, int num, int new_mark, tree *root_main) {
    if (root == NULL) {
        return NULL;
    }
    double new_mid_value = 0;
    for (int i = 0; i < root->quantity_students; i++) {          //идем по массиву стедентов в дереве
        if (root->students[i].name == name) {                //если нашли, то меняем нужную оценку, пересчитываем среднюю и сново добавляем его в дерево
            root->students[i].marks[num] = new_mark;
            for (int k = 0; k < amount_marks; k++) {
                new_mid_value += root->students[i].marks[k];
            }
            root->students[i].middle_value = new_mid_value/amount_marks;
            root = insert(root, root->students[i]);
            root->quantity_students--;

            //после чего чистим старые значения
            if (root->quantity_students == 0) {          //проверяем, если студентов в ноде больше не осталось, то удаляем нод
                return remove_node(root_main, root->value);
            } else {                                        //иначе перевыделяем память под массив студентов меня местами последнего студента с текущим
                student swap_stud = root->students[i];
                root->students[i] = root->students[root->quantity_students];
                root->students[root->quantity_students] = swap_stud;
                root->students = (student*)realloc(root->students, sizeof(student)*root->quantity_students);
                if (root->students == NULL) {               //Если система не дала памяти под массив, выходим
                    printf("No memory");
                    return NULL;
                }
                return root_main;
            }
        }
    }
    if (change(root->left_child, name, num, new_mark, root_main)) {
        return root_main;
    }
    if (change(root->right_child, name, num, new_mark, root_main)) {
        return root_main;
    }
    return NULL;
}

int counting_good_students (tree *root, double value) {
    if (root == NULL) {
        return 0;
    }
    int count = 0;
    if (root->value > value || equally_double(value, root->value)) {
        count += root->quantity_students;
    }
    count += counting_good_students(root->left_child, value) + counting_good_students(root->right_child, value);
    return  count;
}

int counting_students (tree *root) {
    if (root == NULL) {
        return 0;
    }
    int count = root->quantity_students;
    count += counting_students(root->left_child) + counting_students(root->right_child);
    return  count;
}

int find_with_flag(tree *root, double del_middle_value, double *value_for_remove) {
    if (root == NULL) {
        return 0;
    } else if (root->value < del_middle_value) {
        *value_for_remove = root->value;
        return 1;
    } else {
        return find_with_flag(root->left_child, del_middle_value, value_for_remove);
    }
}

void remove_all_living_things(tree *root) {
    if (root == NULL)
        return;
    else {
        if (root->left_child != NULL) {
            remove_all_living_things(root->left_child);
        }
        if (root->right_child != NULL) {
            remove_all_living_things(root->right_child);
        }
        free(root->students);
        free(root);
    }
}

int main() {
    FILE *input, *output;
    input = fopen("input.txt", "r");
    tree* root = NULL;
    student stud;
    char str_rec[max];
    fscanf(input, "%s", str_rec);
    fscanf(input, "%s", str_rec);
    while (strncmp(str_rec, "CHANGES:", 8)) {
        double mid_val = 0;
        stud.name = char_in_int(str_rec);
        for (int i = 0; i < amount_marks; i++) {
            fscanf(input, "%lf", &stud.marks[i]);
            mid_val += stud.marks[i];
        }
        stud.middle_value = mid_val/amount_marks;
        root = insert(root, stud);
        fscanf(input, "%s", str_rec);
    }
    fscanf(input, "%s", str_rec);
    while (strncmp(str_rec, "BORDERS:", 8)) {
        int name, num, new_mark;
        name = char_in_int(str_rec);
        fscanf(input, "%d %d", &num, &new_mark);
        root = change(root, name, num, new_mark, root);
        fscanf(input, "%s", str_rec);
    }
    double del_middle_value;
    fscanf(input, "%lf", &del_middle_value);
    double min_middle_value;
    fscanf(input, "%lf", &min_middle_value);
    double value_for_remove = 0;
    while (find_with_flag(root, del_middle_value, &value_for_remove)) {
        root = remove_node(root, value_for_remove);
    }
    fclose(input);
    output = fopen("output.txt", "w");
    fprintf(output, "%d %d", counting_students(root), counting_good_students(root, min_middle_value));
    remove_all_living_things(root);
    fclose(output);
    return 0;
}
