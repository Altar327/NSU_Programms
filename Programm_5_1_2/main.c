#include <stdio.h>
#include <stdlib.h>
#define start_node 0

typedef struct Node_ {          //Вершины графа
    int name;
    int key;
} Node;


typedef struct list_ {               //Список смежности
    int value;
    int name_of_the_next_node;
    struct list_ *child;
} list;


void swap_node (Node *a, Node *b) {
    Node c = *a;
    *a = *b;
    *b = c;
}

list* del_from_list_first_value (list *head, int value) {
    list *prev = NULL;
    list *curr = head;
    while (curr) {
        if (curr->value == value) {
            if (prev) {
                prev->child = curr->child;
                free(curr);
                return head;
            } else {
                head = curr->child;
//                curr->child = NULL;
                free(curr);
                return head;
            }
        }
        prev = curr;
        curr = curr->child;
    }
}

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

int main() {
    int final_node, count_node, count_edge;
    scanf("%d %d %d", &final_node, &count_node, &count_edge);

    list **lists_adjacency = (list**)malloc(count_node * sizeof(list*));           //массив листов смежности
    for (int i = 0; i < count_node; i++) {
        lists_adjacency[i] = NULL;
    }

    int x, y, way;              //Заполняем листы смежности
    for (int i = 0; i < count_edge; i++) {
        scanf("%d %d %d", &x, &y, &way);
        if (lists_adjacency[x] != NULL) {
            list* list_for_search = lists_adjacency[x];
            while (list_for_search->child != NULL) {
                list_for_search = list_for_search->child;
            }
            list_for_search->child = (list *) malloc(sizeof(list));
            if (list_for_search->child) {
                list_for_search->child->name_of_the_next_node = y;
                list_for_search->child->value = way;
                list_for_search->child->child = NULL;
            }
        } else {
            lists_adjacency[x] = (list *) malloc(sizeof(list));
            lists_adjacency[x]->name_of_the_next_node = y;
            lists_adjacency[x]->value = way;
            lists_adjacency[x]->child = NULL;
        }
    }

    int* keys = (int*)calloc(count_node, sizeof(int));
    //Массив с текущим "значением дастяжения вершин" (назаву так длину пути до i вершины, где i место ключа в массиве)
    //Будем сичтать, что вершина не открыта если еее ключ равен 0, за исключением стартовой.

    int **array_ways_to_the_nodes = (int**)malloc(count_node * sizeof(int*));
    for (int i = 0; i < count_node; i++) {
        array_ways_to_the_nodes[i] = (int *)calloc(count_node + 1, sizeof(int));
    }
    //Массив, внутри которого массивы описывающие путь до вершины i, при этом условимся что на 0 месте будет стоять кол-во элементов в этом массиве

    Node *queue = (Node *)malloc(count_node * sizeof(Node));               //Куча вершин
    int queue_size = 1;                                             //Кол-во элементов в очереди

    Node first_node;
    first_node.name = start_node;
    first_node.key = 0;
    array_ways_to_the_nodes[first_node.name][0] = 1;
    array_ways_to_the_nodes[first_node.name][1] = 0;
    keys[start_node] = 0;

    *queue = first_node;

    while (queue_size) {
        Node *curr_node = queue;
        if (lists_adjacency[curr_node->name] == NULL) {
            queue = remove_from_queue(queue, &queue_size);
        } else if (curr_node) {

            int min_value = lists_adjacency[curr_node->name]->value;        //Переменные для поиска минимального пути из вершины
            int min_name = lists_adjacency[curr_node->name]->name_of_the_next_node;


            list *list_for_search = lists_adjacency[curr_node->name]->child;                //Переменная чтобы идти по списку смежности
            while (list_for_search) {               //Ищем минимальный элемент, проходя по всему списку
                if (min_value > list_for_search->value) {
                    min_value = list_for_search->value;
                    min_name = list_for_search->name_of_the_next_node;
                }
                list_for_search = list_for_search->child;
            }

            if (keys[min_name] > min_value + curr_node->key) {           //Открываем или переоткрываем вершину
                keys[min_name] = min_value + curr_node->key;
                Node *new_node = queue;
                while (new_node->name != min_name) {
                    new_node++;
                }
                new_node->key = keys[min_name];
                for (int i = 0; i <= array_ways_to_the_nodes[curr_node->name][0]; i++) {
                    array_ways_to_the_nodes[min_name][i] = array_ways_to_the_nodes[curr_node->name][i];
                }
                array_ways_to_the_nodes[min_name][0] += 1;
                array_ways_to_the_nodes[min_name][array_ways_to_the_nodes[min_name][0]] = min_name;

            } else if (keys[min_name] == 0 && min_name != 0) {
                keys[min_name] = min_value + curr_node->key;

                for (int i = 0; i <= array_ways_to_the_nodes[curr_node->name][0]; i++) {
                    array_ways_to_the_nodes[min_name][i] = array_ways_to_the_nodes[curr_node->name][i];
                }
                array_ways_to_the_nodes[min_name][0] += 1;
                array_ways_to_the_nodes[min_name][array_ways_to_the_nodes[min_name][0]] = min_name;

                Node new_node;
                new_node.key = keys[min_name];
                new_node.name = min_name;

                queue = insert_in_queue(new_node, queue, &queue_size);
            }
            lists_adjacency[curr_node->name] = del_from_list_first_value(lists_adjacency[curr_node->name], min_value);         //удаляем путь по которому прошлись
        }
    }

//    for (int i = 0; i < count_node; i++) {
//        printf("%d ", keys[i]);
//    }

    printf("%d\n", keys[final_node]);
    for (int i = 1; i <= array_ways_to_the_nodes[final_node][0]; i++) {
        printf("%d ", array_ways_to_the_nodes[final_node][i]);
    }

    for (int i = 0; i < count_node; i++) {
        free(lists_adjacency[i]);
        free(array_ways_to_the_nodes[i]);
    }
    free(array_ways_to_the_nodes);
    free(lists_adjacency);
    free(queue);

    return 0;
}
