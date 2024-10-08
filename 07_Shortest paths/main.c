#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct node {
    int vertex;
    int weight;
    struct node* next;
} Node;

struct node_info {
    int vertex;
    int distance;
};

Node* create_node(int vertex, int weight);
void print_path(int* path, int count, int distance);
int impl(Node** graph, int verticies, int start, int end, int n);
int check_graph(Node** graph, int first_vertex, int second_vertex, int op);
int delete(Node** graph, int first_vertex, int second_vertex, int verticies, int n);
int insert(Node** graph, int first_vertex, int second_vertex, int weight, int verticies, int n);
int update(Node** graph, int first_vertex, int second_vertex, int weight, int verticies, int n);


void print_new_line(int* n) {
    if ((*n)++ == 0) {
        return;
    }
    printf("\n");
}

int main() {
    char command;
    int verticies, edges, n = 0, first_vertex, second_vertex, weight;
    scanf("%d %d", &verticies, &edges);
    Node** graph = (Node**)malloc(verticies * sizeof(Node*));
    if (graph == NULL) {
        return 1;
    }
    
    for (int i = 0; i < verticies; i++) {
        graph[i] = NULL;
    }
    
    for (int i = 0; i < edges; i++) {
        scanf(" (%d,%d,%d)", &first_vertex, &second_vertex, &weight);
        n = insert(graph, first_vertex, second_vertex, weight, verticies, n);
    }
    while (scanf(" %c ", &command) != EOF) {
        switch (command) {
            case 'i': {
                scanf(" %d %d %d", &first_vertex, &second_vertex, &weight);
                n = insert(graph, first_vertex, second_vertex, weight, verticies, n);
                break;
            }
            case 's': {
                scanf(" %d %d", &first_vertex, &second_vertex);
                n = impl(graph, verticies, first_vertex, second_vertex, n);
                break;
            }
            case 'u': {
                scanf(" %d %d %d", &first_vertex, &second_vertex, &weight);
                n = update(graph, first_vertex, second_vertex, weight, verticies, n);
                break;
            }
            case 'd': {
                scanf(" %d %d", &first_vertex, &second_vertex);
                n = delete(graph, first_vertex, second_vertex, verticies, n);
                break;
            }
            default: {
                printf("%c failed\n", command);
                break;
            }
        }
    }
    return 0;
}

int insert(Node** graph, int first_vertex, int second_vertex, int weight, int verticies, int n) {
    if (first_vertex < 0 || second_vertex < 0 || first_vertex >= verticies || second_vertex >= verticies || first_vertex == second_vertex) {
        print_new_line(&n);
        printf("insert %d %d failed", first_vertex, second_vertex);
        return n;
    }
    if (!check_graph(graph, first_vertex, second_vertex, 1)) {
        print_new_line(&n);
        printf("insert %d %d failed", first_vertex, second_vertex);
        return n;
    }
    Node* new_node = create_node(second_vertex, weight);
    new_node->next = graph[first_vertex];
    graph[first_vertex] = new_node;

    new_node = create_node(first_vertex, weight);
    new_node->next = graph[second_vertex];
    graph[second_vertex] = new_node;
    return n;
}

int delete(Node** graph, int first_vertex, int second_vertex, int verticies, int n) {
    if (first_vertex < 0 || second_vertex < 0 || first_vertex >= verticies || second_vertex >= verticies || first_vertex == second_vertex) {
        print_new_line(&n);
        printf("delete %d %d failed", first_vertex, second_vertex);
        return n;
    }
    int present = 0;
    Node* prev = NULL;
    Node* temp = graph[first_vertex];
    while (temp != NULL) {
        if (temp->vertex != second_vertex) {
            prev = temp;
            temp = temp->next;
            continue;
        }
        present = 1;
        break;
    }
    if (!present) {
        print_new_line(&n);
        printf("delete %d %d failed", first_vertex, second_vertex);
        return n;
    }
    if (prev == NULL) {
        graph[first_vertex] = temp->next;
    }
    else {
        prev->next = temp->next;
    }
    free(temp);
    prev = NULL;
    temp = graph[second_vertex];
    while (temp != NULL) {
        if (temp->vertex != first_vertex) {
            prev = temp;
            temp = temp->next;
            continue;
        }
        if (prev == NULL) {
            graph[second_vertex] = temp->next;
            break;
        }
        prev->next = temp->next;
        free(temp);
        break;
    }
    return n;
}

int update(Node** graph, int first_vertex, int second_vertex, int weight, int verticies, int n) {
    if (!check_graph(graph, first_vertex, second_vertex, 2)) {
        print_new_line(&n);
        printf("update %d %d failed", first_vertex, second_vertex);
        return n;
    }
    Node* temp = graph[first_vertex];
    while (temp != NULL) {
        if (temp->vertex != second_vertex) {
            temp = temp->next;
            continue;
        }
        int new_weight = temp->weight + weight;
        if (new_weight < 0) {
            print_new_line(&n);
            printf("update %d %d failed", first_vertex, second_vertex);
            break;
        }
        temp->weight = new_weight;
        break;
    }
    temp = graph[second_vertex];
    while (temp != NULL) {
        if (temp->vertex != first_vertex) {
            temp = temp->next;
            continue;
        }
        int new_weight = temp->weight + weight;
        if (new_weight < 0) {
            break;
        }
        temp->weight = new_weight;
        break;
    }
    return n;
}

Node* create_node(int vertex, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->next = NULL;
    newNode->vertex = vertex;
    newNode->weight = weight;
    return newNode;
}

int check_graph(Node** graph, int first_vertex, int second_vertex, int op) {
    Node* temp = graph[first_vertex];
    while (temp != NULL) {
        if (temp->vertex != second_vertex) {
            temp = temp->next;
            continue;
        }
        if (op == 1) {
            return 0;
        } else if (op == 2) {
            return 1;
        }
    }
    if (op == 1) {
        return 1;
    }
}

int impl(Node** graph, int verticies, int start, int end, int n) {
    if (start < 0 || end < 0 || start >= verticies || end >= verticies || end == start) {
        print_new_line(&n);
        printf("search failed");
        return n;
    }
    int* distance = (int*)malloc(verticies * sizeof(int));
    int* predecessor = (int*)malloc(verticies * sizeof(int));
    int* visited = (int*)malloc(verticies * sizeof(int));

    for (int i = 0; i < verticies; ++i) {
        visited[i] = 0;
        predecessor[i] = -1;
        distance[i] = INT_MAX;
    }

    struct node_info** heap = (struct node_info**)malloc(verticies * sizeof(struct node_info*));
    for (int i = 0; i < verticies; ++i) {
        heap[i] = (struct node_info*)malloc(sizeof(struct node_info));
        heap[i]->vertex = i;
        heap[i]->distance = INT_MAX;
    }
    heap[start]->distance = distance[start] = 0;
    while (1) {
        int u = -1;
        int min_dst = INT_MAX;
        for (int i = 0; i < verticies; ++i) {
            if (visited[i]) {
                continue;
            }
            if (heap[i]->distance >= min_dst) {
                continue;
            }
            u = i;
            min_dst = heap[i]->distance;
        }
        if (u == -1 || visited[end]) {
            break;
        }
        visited[u] = 1;
        Node* temp = graph[u];
        while (temp != NULL) {
            if (temp->vertex >= verticies || temp->vertex < 0) {
                temp = temp->next;
                continue;
            }
            int v = temp->vertex;
            int weight = temp->weight;

            if (visited[v] || distance[u] == INT_MAX || distance[u] + weight >= distance[v]) {
                temp = temp->next;
                continue;
            }
            predecessor[v] = u;
            heap[v]->distance = distance[v] = distance[u] + weight;
            temp = temp->next;
        }
    }

    if (distance[end] == INT_MAX) {
        print_new_line(&n);
        printf("search %d %d failed", start, end);
    } else {
        print_new_line(&n);
        int count = 0;
        int vert = end;
        int* path = (int*)malloc(verticies * sizeof(int));
        while (vert != -1) {
            path[count++] = vert;
            vert = predecessor[vert];
        }
        print_path(path, count, distance[end]);
        free(path);
    }
    for (int i = 0; i < verticies; ++i) {
        free(heap[i]);
    }
    free(heap);
    free(visited);
    free(distance);
    free(predecessor);
    return n;
}

void print_path(int* path, int count, int distance) {
    printf("%d: [", distance);
    for (int i = count - 1; i >= 0; --i) {
        printf("%d%s", path[i], i == 0 ? "" : ", ");
    }
    printf("]");
}
