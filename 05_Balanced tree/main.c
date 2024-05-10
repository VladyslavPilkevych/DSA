#include <stdlib.h>
#include <stdio.h>

int atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

struct Data {
    int id;
    char text[50];
};

typedef struct AVLNode {
    struct Data data;
    int balance;

    struct AVLNode* leftChild;
    struct AVLNode* rightChild;
} AVLNode;

struct AVLTree {
    struct TreeNode *root;
};

struct AVLNode* createAVLNode(struct Data data) {
    struct AVLNode* newNode = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    newNode->data = data;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->balance = 1;
    return newNode;
}

struct AVLTree* createAVLTree() {
    struct AVLTree* newTree = (struct AVLTree*)malloc(sizeof(struct AVLTree));
    newTree->root = NULL;
    return newTree;
}

int getHeight(AVLNode* node) {
    return (node == NULL ? 0 : node->balance);
}

int getBalanceFactor(AVLNode* node) {
    return node == NULL ? 0 : getHeight(node->leftChild) - getHeight(node->rightChild);
}

AVLNode* rightRotate(AVLNode* node) {
    AVLNode* nodeLeftChild = node->leftChild;
    node->leftChild = nodeLeftChild->rightChild;
    nodeLeftChild->rightChild = node;
    node->balance = (getHeight(node->leftChild) > getHeight(node->rightChild) ? getHeight(node->leftChild) : getHeight(node->rightChild)) + 1;
    nodeLeftChild->balance = (getHeight(nodeLeftChild->leftChild) > getHeight(nodeLeftChild->rightChild) ? getHeight(nodeLeftChild->leftChild) : getHeight(nodeLeftChild->rightChild)) + 1;
    return nodeLeftChild;
}

AVLNode* leftRotate(AVLNode* node) {
    AVLNode* nodeRightChild = node->rightChild;
    node->rightChild = nodeRightChild->leftChild;
    nodeRightChild->leftChild = node;
    node->balance = (getHeight(node->leftChild) > getHeight(node->rightChild) ? getHeight(node->leftChild) : getHeight(node->rightChild)) + 1;
    nodeRightChild->balance = (getHeight(nodeRightChild->leftChild) > getHeight(nodeRightChild->rightChild) ? getHeight(nodeRightChild->leftChild) : getHeight(nodeRightChild->rightChild)) + 1;
    return nodeRightChild;
}

AVLNode* AVLBalanceNodeInsert(AVLNode* node, int data) {
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1) {
        if (data > node->leftChild->data.id)
            node->leftChild = leftRotate(node->leftChild);
        return rightRotate(node);
    } else if (balanceFactor < -1) {
        if (data < node->rightChild->data.id)
            node->rightChild = rightRotate(node->rightChild);
        return leftRotate(node);
    }
    return NULL;
}
AVLNode* insert(AVLNode* node, struct Data data) {
    if (node == NULL)
        return createAVLNode(data);
    if (data.id < node->data.id)
        node->leftChild = insert(node->leftChild, data);
    else if (data.id > node->data.id)
        node->rightChild = insert(node->rightChild, data);
    else
        return node;
    node->balance = (getHeight(node->leftChild) > getHeight(node->rightChild) ? getHeight(node->leftChild) : getHeight(node->rightChild)) + 1;
    AVLNode* returnResult = AVLBalanceNodeInsert(node, data.id);
    return returnResult == NULL ? node : returnResult;
}
AVLNode* search(AVLNode* node, int id) {
    while (node != NULL) {
        if (id == node->data.id)
            return node;
        else if (id < node->data.id)
            node = node->leftChild;
        else
            node = node->rightChild;
    }
    return NULL;
}
AVLNode* findMinimalNode(AVLNode* node) {
    if (node == NULL)
        return NULL;
    AVLNode* temp = node;
    while (temp->leftChild != NULL)
        temp = temp->leftChild;
    return temp;
}
AVLNode* AVLBalanceNodeDelete(AVLNode* node) {
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1) {
        if (getBalanceFactor(node->leftChild) < 0)
            node->leftChild = leftRotate(node->leftChild);
        return rightRotate(node);
    } else if (balanceFactor < -1) {
        if (getBalanceFactor(node->rightChild) > 0)
            node->rightChild = rightRotate(node->rightChild);
        return leftRotate(node);
    }
    return NULL;
}
AVLNode* deleteNode(AVLNode* node, int id) {
    if (node == NULL)
        return NULL;
    if (id < node->data.id)
        node->leftChild = deleteNode(node->leftChild, id);
    else if (id > node->data.id)
        node->rightChild = deleteNode(node->rightChild, id);
    else {
        AVLNode* temp;
        if (node->leftChild == NULL || node->rightChild == NULL) {
            if (node->leftChild == NULL)
                temp = node->rightChild;
            else
                temp = node->leftChild;
            free(node);
            return temp;
        } else {
            temp = findMinimalNode(node->rightChild);
            node->data = temp->data;
            node->rightChild = deleteNode(node->rightChild, temp->data.id);
        }
    }
    if (node == NULL)
        return NULL;
    node->balance = (getHeight(node->leftChild) > getHeight(node->rightChild) ? getHeight(node->leftChild) : getHeight(node->rightChild)) + 1;
    AVLNode* returnResult = AVLBalanceNodeDelete(node);
    return returnResult == NULL ? node : returnResult;
}
void searchRangeHelper(AVLNode* node, int low, int high, int* flag) {
    if (node == NULL)
        return;

    if (node->data.id > low)
        searchRangeHelper(node->leftChild, low, high, flag);

    if (node->data.id >= low && node->data.id <= high) {
        if ((*flag) != 0) printf("\n");
        printf("%d %s", node->data.id, node->data.text);
        *flag = 1;
    }

    if (node->data.id < high)
        searchRangeHelper(node->rightChild, low, high, flag);
}

void searchRange(AVLNode* root, int low, int high, int* flag) {
    searchRangeHelper(root, low, high, flag);
}
void freeAVLTree(AVLNode* node) {
    if (node != NULL) {
        freeAVLTree(node->leftChild);
        freeAVLTree(node->rightChild);
        free(node);
    }
}
int main(){
    AVLNode* root = NULL;

    char command, text[50], id[20], cmd[2], num1[10], num2[10] = {'\0'};
    int low, high, flag = 0, count = 0;

    while (fgets(text, sizeof text, stdin) != NULL) {
        switch (text[0]) {
            case 'i': {
                struct Data data;
                sscanf(text, "%s %s %[^\n]", cmd, id, data.text);
                data.id = atoi(id);

                root = insert(root, data);
                break;
            }
            case 's': {
                num2[0] = '\0';
                sscanf(text, "%s %s %s", cmd, num1, num2);

                if (num2[0] == '\0') {
                    AVLNode *foundNode = search(root, atoi(num1));
                    if (foundNode != NULL)
                    {
                        if (flag != 0) printf("\n");
                        printf("%d %s", foundNode->data.id, foundNode->data.text);
                        flag = 1;
                    }
                } else {
                    if (atoi(num1) < atoi(num2))
                        searchRange(root, atoi(num1), atoi(num2), &flag);
                    else
                        searchRange(root, atoi(num2), atoi(num1), &flag);
                }
                break;
            }
            case 'd': {
                sscanf(text, "%s %s", cmd, num1);
                root = deleteNode(root, atoi(num1));
                break;
            }
            default:
                break;
        }
    }

    freeAVLTree(root);
    return 0;
}
