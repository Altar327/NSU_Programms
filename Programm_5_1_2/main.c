#include <stdio.h>
#include <stdlib.h>
#define start_node 0

typedef struct Node_ {          //Вершины графа
    int name;
    int key;
} Node;


typedef struct List_ {               //Список смежности
    int value;
    int flag;
    int name_of_the_next_node;
    struct List_ *child;
} List;


void swap_node (Node *a, Node *b) {
    Node c = *a;
    *a = *b;
    *b = c;
}

//list* del_from_list_first_value (list *head, int value) {
//    list *prev = NULL;
//    list *curr = head;
//    while (curr) {
//        if (curr->value == value) {
//            if (prev) {
//                prev->child = curr->child;
//                free(curr);
//                return head;
//            } else {
//                head = curr->child;
////                curr->child = NULL;
//                free(curr);
//                return head;
//            }
//        }
//        prev = curr;
//        curr = curr->child;
//    }
//}

void sift_down (int i, Node *queue, int queue_size) {                //Просеивание вниз
    while (2 * i + 1 < queue_size) {
        int left = 2 * i + 1;                                       // left — левый сын
        int right = 2 * i + 2;                                      // right — правый сын
        int j = left;
        if ((right < queue_size) && (queue[right].key < queue[left].key)){
            j = right;
        }
        if (queue[i].key <= queue[j].key) {
            break;
        }
        swap_node(&queue[i], &queue[j]);
        i = j;
    }
}

void sift_up (int i, Node *queue) {                    //Поднятие вверх
    while (queue[i].key < queue[(i - 1) / 2].key) {
        swap_node(&queue[i], &queue[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Node* remove_from_queue(Node *queue, int *queue_size) {
    queue[0] = queue[*queue_size - 1];
    *queue_size -= 1;
    sift_down(0, queue, *queue_size);
    return queue;
}

Node* insert_in_queue (Node new_node, Node* queue, int *queue_size) {
    *queue_size += 1;
    queue = (Node*) realloc(queue, *queue_size * sizeof(Node));
    queue[*queue_size - 1] = new_node;
    sift_up(*queue_size - 1, queue);
    return queue;
}

void go (int node, int* array_prev_node_for_i_node) {
    if (array_prev_node_for_i_node[node] != -1) {
        go(array_prev_node_for_i_node[node], array_prev_node_for_i_node);
        printf("%d ", node);
    } else {
        printf("0 ");
    }
}

int main() {
    int final_node, count_node, count_edge;
    scanf("%d %d %d", &final_node, &count_node, &count_edge);

    List **lists_adjacency = (List**)malloc(count_node * sizeof(List*));           //массив листов смежности
    if (lists_adjacency == NULL) {
        return 0;
    }
    for (int i = 0; i < count_node; i++) {
        lists_adjacency[i] = NULL;
    }

    int x, y, way;              //Заполняем листы смежности
    for (int i = 0; i < count_edge; i++) {
        scanf("%d %d %d", &x, &y, &way);
        if (lists_adjacency[x] != NULL) {
            List* list_for_search = lists_adjacency[x];
            while (list_for_search->child != NULL) {
                list_for_search = list_for_search->child;
            }
            list_for_search->child = (List *) malloc(sizeof(List));
            if (list_for_search->child == NULL) {
                return 0;
            }
            if (list_for_search->child) {
                list_for_search->child->name_of_the_next_node = y;
                list_for_search->child->value = way;
                list_for_search->child->flag = 0;
                list_for_search->child->child = NULL;
            }
        } else {
            lists_adjacency[x] = (List *) malloc(sizeof(List));
            if (lists_adjacency[x] == NULL) {
                return 0;
            }
            lists_adjacency[x]->name_of_the_next_node = y;
            lists_adjacency[x]->value = way;
            lists_adjacency[x]->flag = 0;
            lists_adjacency[x]->child = NULL;
        }
    }

    int* keys = (int*)calloc(count_node, sizeof(int));
    if (keys == NULL) {
        return 0;
    }
    //Массив с текущим "значением дастяжения вершин" (назаву так длину пути до i вершины, где i место ключа в массиве)
    //Будем сичтать, что вершина не открыта если еее ключ равен 0, за исключением стартовой.

//    int **array_ways_to_the_nodes = (int**)malloc(count_node * sizeof(int*));
//    for (int i = 0; i < count_node; i++) {
//        array_ways_to_the_nodes[i] = (int *)calloc(count_node + 1, sizeof(int));
//    }
    int *array_prev_node_for_i_node = (int*) calloc(count_node, sizeof(int));
    if (array_prev_node_for_i_node == NULL) {
        return 0;
    }
    array_prev_node_for_i_node[start_node] = -1;

    //Массив, внутри которого массивы описывающие путь до вершины i, при этом условимся что на 0 месте будет стоять кол-во элементов в этом массиве

    Node *queue = (Node *)malloc(count_node * sizeof(Node));               //Куча вершин
    if (queue == NULL) {
        return 0;
    }
    int queue_size = 1;                                             //Кол-во элементов в очереди

    Node first_node;
    first_node.name = start_node;
    first_node.key = 0;
//    array_ways_to_the_nodes[first_node.name][0] = 1;
//    array_ways_to_the_nodes[first_node.name][1] = 0;
    keys[start_node] = 0;

    *queue = first_node;

    while (queue_size) {
        Node *curr_node = queue;

        int flag = 0;
//        int curr_value = lists_adjacency[curr_node->name]->value;
//        int curr_name = lists_adjacency[curr_node->name]->name_of_the_next_node;
        int curr_value;
        int curr_name;

        List *list_for_search = lists_adjacency[curr_node->name];                //Переменная чтобы идти по списку смежности
        while (list_for_search != NULL) {
            if (list_for_search->flag == 0) {
                curr_value = list_for_search->value;
                curr_name = list_for_search->name_of_the_next_node;
                list_for_search->flag++;
                flag++;
                break;
            }
            list_for_search = list_for_search->child;
        }

        if (!flag) {
            queue = remove_from_queue(queue, &queue_size);
        } else if (curr_node) {

//            list *list_for_search = lists_adjacency[curr_node->name]->child;                //Переменная чтобы идти по списку смежности
//            while (list_for_search) {
//                if (!list_for_search->flag) {
//                    curr_value = list_for_search->value;
//                    curr_name = list_for_search->name_of_the_next_node;
//                    list_for_search->flag++;
//                    break;
//                }
//                list_for_search = list_for_search->child;
//            }

            if (keys[curr_name] > curr_value + curr_node->key) {           //Открываем или переоткрываем вершину
                keys[curr_name] = curr_value + curr_node->key;
                Node *new_node = queue;
                while (new_node->name != curr_name) {
                    new_node++;
                }
                new_node->key = keys[curr_name];
//                for (int i = 0; i <= array_ways_to_the_nodes[curr_node->name][0]; i++) {
//                    array_ways_to_the_nodes[curr_name][i] = array_ways_to_the_nodes[curr_node->name][i];
//                }
//                array_ways_to_the_nodes[curr_name][0] += 1;
//                array_ways_to_the_nodes[curr_name][array_ways_to_the_nodes[curr_name][0]] = curr_name;
                array_prev_node_for_i_node[curr_name] = curr_node->name;

            } else if (keys[curr_name] == 0 && curr_name != 0) {
                keys[curr_name] = curr_value + curr_node->key;

//                for (int i = 0; i <= array_ways_to_the_nodes[curr_node->name][0]; i++) {
//                    array_ways_to_the_nodes[curr_name][i] = array_ways_to_the_nodes[curr_node->name][i];
//                }
//                array_ways_to_the_nodes[curr_name][0] += 1;
//                array_ways_to_the_nodes[curr_name][array_ways_to_the_nodes[curr_name][0]] = curr_name;

                array_prev_node_for_i_node[curr_name] = curr_node->name;


                Node new_node;
                new_node.key = keys[curr_name];
                new_node.name = curr_name;

                queue = insert_in_queue(new_node, queue, &queue_size);
            }
//            lists_adjacency[curr_node->name] = del_from_list_first_value(lists_adjacency[curr_node->name], curr_value);         //удаляем путь по которому прошлись
        }
    }

//    for (int i = 0; i < count_node; i++) {
//        printf("%d ", keys[i]);
//    }

    printf("%d\n", keys[final_node]);
    go(final_node, array_prev_node_for_i_node);

    for (int i = 0; i < count_node; i++) {
        free(lists_adjacency[i]);
//        free(array_prev_node_for_i_node[i]);
    }
    free(array_prev_node_for_i_node);
    free(lists_adjacency);
    free(queue);

    return 0;
}
