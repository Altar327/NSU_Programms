#define _CRT_SECURE_NO_DEPRECATE
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
    double value;
    student* students;
    int quantity_students;
    struct tree_* left_child;
    struct tree_* right_child;
} tree;

int char_in_int (char *count) {                     //функция перевода char в int
    int x = 0;
    int count_len = strlen(count);
    for (int i = 0; i < count_len; i++) {                   //записываем по порядку единицы, десятки и т.д.
        x += (count[count_len - i - 1] - '0')*pow(10, i);
    }
    return x;
}

void insert (student stud, tree** root) {
    if ((*root) == NULL) {
        (*root) = (tree*)malloc(sizeof(tree));
        (*root)->value = stud.middle_value;
        (*root)->students = (student*)malloc(sizeof(student)); /* создаем динамический массив с введенным размером. Просим память у системы */
        *((*root)->students) = stud;
        (*root)->quantity_students = 1;
//        if ((*root)->students == 0) /* Если система не дала памяти под массив, выходим */
//        {
//            printf("No memory");
//            return;
//        }
        (*root)->left_child = NULL;
        (*root)->right_child = NULL;
    }  else {
        if (stud.middle_value > (*root)->value) {
            insert(stud, &((*root)->right_child));
        } else if (fabs((*root)->value - stud.middle_value) < epsilon) {
            (*root)->quantity_students++;
            (*root)->students = (student*)realloc((*root)->students, sizeof(student)*(*root)->quantity_students);
            *((*root)->students + (*root)->quantity_students - 1) = stud;
        } else {
            insert(stud, &((*root)->left_child));
        }
    }
}

tree remove_node_far_right_elem (tree **root) {
    if ((*root)->right_child != NULL) {
        return remove_node_far_right_elem(&((*root)->right_child));
    } else if ((*root)->left_child != NULL) {
        tree* tree_for_swap = (tree*)malloc(sizeof(tree));
        tree_for_swap->students = (student*)malloc(sizeof(student)*(*root)->quantity_students);
        memcpy(tree_for_swap, *root, sizeof(tree));
        **root = *((*root)->left_child);
//        free((*root)->left_child);
        return *tree_for_swap;
    } else {
        tree* tree_for_swap = (tree*)malloc(sizeof(tree));
        tree_for_swap->students = (student*)malloc(sizeof(student)*(*root)->quantity_students);
        memcpy(tree_for_swap, *root, sizeof(tree));
        tree_for_swap->right_child = NULL;
        free(*root);
        return *tree_for_swap;
    }
}

tree remove_node_far_left_elem (tree **root) {
    if ((*root)->left_child != NULL) {
        return remove_node_far_left_elem(&((*root)->left_child));
    } else if ((*root)->right_child != NULL) {
        tree* tree_for_swap = (tree*)malloc(sizeof(tree));
        tree_for_swap->students = (student*)malloc(sizeof(student)*(*root)->quantity_students);
        memcpy(tree_for_swap, *root, sizeof(tree));
        **root = *((*root)->right_child);
//        free((*root)->right_child);
        return *tree_for_swap;
    } else {
        tree* tree_for_swap = (tree*)malloc(sizeof(tree));
        tree_for_swap->students = (student*)malloc(sizeof(student)*(*root)->quantity_students);
        memcpy(tree_for_swap, *root, sizeof(tree));
        tree_for_swap->left_child = NULL;
        free(*root);
        return *tree_for_swap;
    }
}

int find_element_for_remove (tree **root, double value) {
    if ((fabs(value - (*root)->value) < epsilon) && ((*root)->right_child != NULL)) {
        tree tree_for_swap = remove_node_far_left_elem(&((*root)->right_child));
        (*root)->value = tree_for_swap.students->middle_value;
//        (*root)->students = (student*)realloc((*root)->students, sizeof(student)*tree_for_swap.quantity_students);
        (*root)->students = tree_for_swap.students;
        (*root)->quantity_students = tree_for_swap.quantity_students;
        (*root)->right_child = tree_for_swap.right_child;
    } else if ((fabs(value - (*root)->value) < epsilon) && (*root)->left_child != NULL) {
        tree tree_for_swap = remove_node_far_right_elem(&((*root)->left_child));
        (*root)->value = tree_for_swap.students->middle_value;
//        (*root)->students = (student*)realloc((*root)->students, sizeof(student)*tree_for_swap.quantity_students);
        (*root)->students = tree_for_swap.students;
        (*root)->quantity_students = tree_for_swap.quantity_students;
        (*root)->left_child = tree_for_swap.left_child;
    } else if ((fabs(value - (*root)->value) < epsilon) && (*root)->left_child == NULL && ((*root)->right_child == NULL)) {
        free((*root)->students);
        *root = NULL;
        return 1;
    } else  if ((*root)->value > value) {
        if (find_element_for_remove(&((*root)->left_child), value)) {
            (*root)->left_child = NULL;
        }
    } else if ((*root)->value < value) {
        if (find_element_for_remove(&((*root)->right_child), value)) {
            (*root)->right_child = NULL;
        }
    }
    return 0;
}



void remove_all_living_things(tree **root) {
    if (*root == NULL)
        return;
    else {
        if ((*root)->left_child != NULL) {
            remove_all_living_things(&(*root)->left_child);
        }
        if ((*root)->right_child != NULL) {
            remove_all_living_things(&(*root)->right_child);
        }
        free((*root)->students);
        free(*root);
    }
}

int change (tree** root, int name, int num, int new_mark, tree **root_main) {
    if (*root == NULL) {
        return 0;
    }
    double new_mid_value = 0;
    for (int i = 0; i < (*root)->quantity_students; i++) {          //идем по массиву стедентов в дереве
        if ((*root)->students[i].name == name) {                //если нашли, то меняем нужную оценку, пересчитываем среднюю и сново добавляем его в дерево
            (*root)->students[i].marks[num] = new_mark;
            for (int k = 0; k < amount_marks; k++) {
                new_mid_value += (*root)->students[i].marks[k];
            }
            (*root)->students[i].middle_value = new_mid_value/amount_marks;
            insert ((*root)->students[i], root_main);
            (*root)->quantity_students--;
                                            //после чего чистим старые значения
            if ((*root)->quantity_students == 0) {          //проверяем, если студентов в ноде больше не осталось, то удаляем нод
                find_element_for_remove(root_main, (*root)->value);
            } else {                                        //иначе перевыделяем память под массив студентов меня местами последнего студента с текущим
                student swap_stud = (*root)->students[i];
                (*root)->students[i] = (*root)->students[(*root)->quantity_students];
                (*root)->students[(*root)->quantity_students] = swap_stud;
                (*root)->students = (student*)realloc((*root)->students, sizeof(student)*(*root)->quantity_students);
            }
            return 1;
        }
    }
    if (change(&((*root)->left_child), name, num, new_mark, root_main)) {
        return 1;
    }
    if (change(&((*root)->right_child), name, num, new_mark, root_main)) {
        return 1;
    }
    return 0;
}

int counting_good_students (tree **root, double value) {
    if (*root == NULL) {
        return 0;
    }
    int count = 0;
    if ((*root)->value > value || fabs(value - (*root)->value) < epsilon) {
        count += (*root)->quantity_students;
    }
    count += counting_good_students(&((*root)->left_child), value) + counting_good_students(&((*root)->right_child), value);
    return  count;
}

int counting_students (tree **root) {
    if (*root == NULL) {
        return 0;
    }
    int count = (*root)->quantity_students;
    count += counting_students(&((*root)->left_child)) + counting_students(&((*root)->right_child));
    return  count;
}

int find_with_flag(tree **root, double del_middle_value, double *value_for_remove) {
    if (*root == NULL) {
        return 0;
    } else if ((*root)->value < del_middle_value) {
        *value_for_remove = (*root)->value;
        return 1;
    } else {
        return find_with_flag(&((*root)->left_child), del_middle_value, value_for_remove);
    }
}

int main() {
    FILE *input, *output;
    input = fopen("input.txt", "r");
    tree* root = NULL;
    student stud;
    char str_rec[max];
    fscanf(input, "%s", &str_rec);
    fscanf(input, "%s", &str_rec);
    while (strncmp(str_rec, "CHANGES:", 8)) {
        double mid_val = 0;
        stud.name = char_in_int(str_rec);
        for (int i = 0; i < amount_marks; i++) {
            fscanf(input, "%lf", &stud.marks[i]);
            mid_val += stud.marks[i];
        }
        stud.middle_value = mid_val/amount_marks;
        insert (stud, &root);
        fscanf(input, "%s", &str_rec);
    }
    fscanf(input, "%s", &str_rec);
    while (strncmp(str_rec, "BORDERS:", 8)) {
        int name, num, new_mark;
        name = char_in_int(str_rec);
        fscanf(input, "%d %d", &num, &new_mark);
        change(&root, name, num, new_mark, &root);
        fscanf(input, "%s", &str_rec);
    }
    double del_middle_value;
    fscanf(input, "%lf", &del_middle_value);
    double min_middle_value;
    fscanf(input, "%lf", &min_middle_value);
    double value_for_remove = 0;
    while (find_with_flag(&root, del_middle_value, &value_for_remove)) {
        find_element_for_remove(&root, value_for_remove);
    }
    fclose(input);
    output = fopen("output.txt", "w");
    fprintf(output, "%d %d", counting_students(&root), counting_good_students(&root, min_middle_value));
    remove_all_living_things(&root);
    fclose(output);
    return 0;
}