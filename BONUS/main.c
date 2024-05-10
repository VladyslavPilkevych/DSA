#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

int findMissingNumber(int arr[], int N) {
    int expectedSum = 0;
    for (int i = -N/4; i <= 3*N/4; i++) {
        expectedSum += i;
    }
    
    int actualSum = 0;
    for (int i = 0; i < N; i++) {
        actualSum += arr[i];
    }
    return expectedSum - actualSum;
}

int main() {
    int N = 10;
    int size = 0;
    int* arr = (int*)malloc((N) * sizeof(int));
    
    char buffer[100];
    while (fgets(buffer, 100, stdin) != NULL) {
        arr[size] = atoi(buffer);

        if (++size == N) {
            N *= 2;
            arr = (int*)realloc(arr, N * sizeof(int));
        }
    }
    
    int missingNumber = findMissingNumber(arr, size);
    printf("%d\n", missingNumber);

    free(arr);
    
    return 0;
}
