#include <stdlib.h>
#include <stdio.h>

int stringLength(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* stringCopy(const char *source) {
    int len = stringLength(source);
    char *copy = (char *)malloc((len + 1) * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        copy[i] = source[i];
    }
    copy[len] = '\0';
    return copy;
}

typedef struct ChainingNode
{
    char *key;
    char *value;
    int hash;
    struct ChainingNode *next;
} ChainingNode;

typedef struct
{
    int size;
    int capacity;
    float threshold;
    float loadFactor;
    ChainingNode **hashTable;
} HashMapChaining;

ChainingNode *createChainingNode(char *key, char *value, int hash)
{
    ChainingNode *newNode = (ChainingNode *)malloc(sizeof(ChainingNode));
    newNode->key = stringCopy(key);
    newNode->value = stringCopy(value);
    newNode->hash = hash;
    newNode->next = NULL;
    return newNode;
}

HashMapChaining initHashMapChaining(int capacity, float loadFactor)
{
    HashMapChaining hashMap;
    hashMap.size = 0;
    hashMap.capacity = capacity;
    hashMap.loadFactor = loadFactor;
    hashMap.threshold = capacity * loadFactor;
    hashMap.hashTable = (ChainingNode **)calloc(capacity, sizeof(ChainingNode *));
    for (int i = 0; i < capacity; i++) {
        hashMap.hashTable[i] = NULL;
    }
    return hashMap;
}

int hash(const char *key)
{
    int hashInt = 5381;
    int c;
    while ((c = *key++))
        hashInt = ((hashInt << 5) + hashInt) + c;
    return hashInt % 3000;
}


int hash_index(int hash, int capacity)
{
    return (capacity - 1) & hash;
}

int compare(ChainingNode *node, const char *key, int hash)
{
    return (node != NULL && node->hash == hash && (node->key == key || *(int *)key == *(int *)node->key));
}

void resize(HashMapChaining* hashMap) {
    int oldCapacity = hashMap->capacity;
    ChainingNode** oldTable = hashMap->hashTable;

    hashMap->capacity += 100;
    hashMap->threshold = hashMap->capacity * hashMap->loadFactor;
    hashMap->hashTable = (ChainingNode**)calloc(hashMap->capacity, sizeof(ChainingNode*));

    for (int i = 0; i < oldCapacity; i++) {
        ChainingNode* node = oldTable[i];
        while (node != NULL) {
            ChainingNode* next = node->next;
            int idx = hash_index(node->hash, hashMap->capacity);
            node->next = hashMap->hashTable[idx];
            hashMap->hashTable[idx] = node;
            node = next;
        }
    }

    free(oldTable);
}

void insert(HashMapChaining hashMap, char *key, char *value)
{
    int hashVal = hash(key);
    int idx = hash_index(hashVal, hashMap.capacity);

    ChainingNode *node = hashMap.hashTable[idx];

    if (node == NULL) {
        hashMap.hashTable[idx] = createChainingNode(key, value, hashVal);
    } else
    {
        while (1)
        {
            if (compare(node, key, hashVal))
            {
                printf("\ninsert failed");
                break;
            }
            if (node->next == NULL)
            {
                node->next = createChainingNode(key, value, hashVal);
                break;
            }
            node = node->next;
        }
    }
    if (++(hashMap.size) > hashMap.threshold)
        resize(&hashMap);
}

void update(HashMapChaining hashMap, char *key, char *value)
{
    int hashVal = hash(key);
    int idx = hash_index(hashVal, hashMap.capacity);

    ChainingNode *node = hashMap.hashTable[idx];

    while (node != NULL)
    {
        if (compare(node, key, hashVal)) {
            double currentValue = atof(node->value);
            double subtractValue = atof(value);
            double newValue = currentValue + subtractValue;

            if (newValue > 0) {
                int n = 0;
                double copy = newValue;
                while(copy > 0){
                    n++;
                    copy/=10;
                }
                free(node->value);
                node->value = (char *)malloc((n + 1) * sizeof(char));
                snprintf(node->value, (n + 1), "%.2f", newValue);

            } else {
                printf("\nupdate failed");
            }
            return;
        }
        node = node->next;
    }
}


void search(HashMapChaining hashMap, char *key)
{
    if (key == NULL)
        return;

    int hashVal = hash(key);
    int idx = hash_index(hashVal, hashMap.capacity);
    int errorFlag = 1;
    ChainingNode *node = hashMap.hashTable[idx];

    while (node != NULL)
    {
        if (compare(node, key, hashVal)) {
            char *valueKopy = stringCopy(node->value);
            for(int i = 0; valueKopy[i] != '\0'; i++) {
                if (valueKopy[i] == '.') {
                    valueKopy[i] = ',';
                    break;
                }
            }
            printf("%s", valueKopy);
            free(valueKopy);
            errorFlag = 0;
            return;
        }
        node = node->next;
    }
    if (errorFlag) printf("search failed");

}

int delete(HashMapChaining hashMap, char *key)
{
    if (key == NULL)
        return 0;

    int hashVal = hash(key);
    int idx = hash_index(hashVal, hashMap.capacity);
    int errorFlag = 1;

    ChainingNode *node = hashMap.hashTable[idx];
    ChainingNode *prev = NULL;

    while (node != NULL)
    {
        if (compare(node, key, hashVal))
        {
            if (prev == NULL)
                hashMap.hashTable[idx] = node->next;
            else
                prev->next = node->next;
            free(node->key);
            free(node->value);
            free(node);
            errorFlag = 0;
            return 1;
        }
        prev = node;
        node = node->next;
    }
    if (errorFlag) printf("\ndelete failed");

    return 0;
}

void clearNode(ChainingNode *node){
    if(node == NULL) 
        return;

    while(node->next != NULL){
        ChainingNode *temp = node->next->next;
        free(node->key);
        free(node->value);
        free(node);
        node = temp;
    }
}

void clearHashMap(HashMapChaining hashMap) {
    for (int i = 0; i < hashMap.capacity; i++) {
        clearNode(hashMap.hashTable[i]);
    }
    free(hashMap.hashTable);
}

int main()
{
    HashMapChaining hashMap = initHashMapChaining(3000, 0.75);

    char text[40], cmd[2], dataHuman[30], formattedString[30], valueStr[30];
    int day, month, year, flag = 1, points = 0, flagValue = 0;

    while (fgets(text, sizeof text, stdin) != NULL)
    {
        switch (text[0])
        {
            case 'i':
            {
                struct ChainingNode data;
                sscanf(text, "%s %99[^0-9] %d.%d.%d %s", cmd, dataHuman, &day, &month, &year, valueStr);
                sprintf(formattedString, "%s%d.%d.%d", dataHuman, day, month, year);
                data.value = valueStr;
                data.key = formattedString;
                points = 0;
                flagValue = 0;
                for(int i = 0; valueStr[i] != '\0'; i++) {
                    if (valueStr[i] == ',') {
                        valueStr[i] = '.';
                        flagValue = 1;
                    } else if (points == 2) {
                        if (valueStr[i] - '0' >= 5)
                            valueStr[i-1]++;
                        valueStr[i] = '\0';
                        break;
                    } else if (flagValue) {
                        points++;
                    }
                }
                insert(hashMap, data.key, data.value);
                break;
            }
            case 's':
            {
                if (flag) flag = 0;
                else printf("\n");
                sscanf(text, "%s %[^0-9] %d.%d.%d", cmd, dataHuman, &day, &month, &year);
                sprintf(formattedString, "%s%d.%d.%d", dataHuman, day, month, year);
                search(hashMap, formattedString);
                break;
            }
            case 'u':
            {
                sscanf(text, "%s %[^0-9] %d.%d.%d %s", cmd, dataHuman, &day, &month, &year, valueStr);
                sprintf(formattedString, "%s%d.%d.%d", dataHuman, day, month, year);
                points = 0;
                flagValue = 0;
                for(int i = 0; valueStr[i] != '\0'; i++) {
                    if (valueStr[i] == ',') {
                        valueStr[i] = '.';
                        flagValue = 1;
                    } else if (points == 2) {
                        if (valueStr[i] - '0' >= 5)
                            valueStr[i-1]++;
                        valueStr[i] = '\0';
                        break;
                    } else if (flagValue) {
                        points++;
                    }
                }
                update(hashMap, formattedString, valueStr);
                break;
            }
            case 'd':
            {
                sscanf(text, "%s %99[^0-9] %d.%d.%d", cmd, dataHuman, &day, &month, &year);
                sprintf(formattedString, "%s%d.%d.%d", dataHuman, day, month, year);
                delete(hashMap, formattedString);
                break;
            }
            default:
                break;
        }
    }
    clearHashMap(hashMap);
    return 0;
}
