#include <stdio.h>
#include <stdlib.h>

typedef struct NUMBER {
    long integer;
    char decimal[11];
} NUMBER;

int main() {
    int N, M;
    int stringSize = 0, lastPosition;
    long mInteger;
    char mDecimal[11];

    scanf("%d", &N);

    NUMBER *numbers = malloc(N * sizeof(NUMBER));

    for (int i = 0; i < N; i++) {
        stringSize = 0;
        int readFromTerminal = scanf("%ld,%s", &numbers[i].integer, numbers[i].decimal);
        if (readFromTerminal == 1) {
            numbers[i].decimal[0] = '0';
            numbers[i].decimal[1] = '\0';
        }

        while (numbers[i].decimal[stringSize] != '\0') stringSize++;
        
        for (int j = stringSize; j <= 10; j++) {
            numbers[i].decimal[j] = '0';
        }
        numbers[i].decimal[10] = '\0';
    }

    int median, left = 0, right = N - 1, isCompleteDecimal = 0;

    scanf("%d", &M);

    for (int i = 0; i < M; i++) {
        if (i)
            printf("\n");

        stringSize = 0;

        int readFromTerminal = scanf("%ld,%s", &mInteger, mDecimal);

        if (readFromTerminal == 1) {
            mDecimal[0] = '0';
            mDecimal[1] = '\0';
        }
            
        while (mDecimal[stringSize] != '\0') stringSize++;

        for (int j = stringSize; j <= 10; j++) {
            mDecimal[j] = '0';
        }
        mDecimal[10] = '\0';

        left = 0;
        right = N - 1;

        while (left <= right) {
            median = (left + right) / 2;

            if (numbers[median].integer == mInteger) {
                int j = 0;
                isCompleteDecimal = 1;

                while (mDecimal[j] != '\0' || numbers[median].decimal[j] != '\0') {
                    if (mDecimal[j] != numbers[median].decimal[j]) {
                        isCompleteDecimal = 0;
                        if (numbers[median].integer < 0)
                            if (mDecimal[j] < numbers[median].decimal[j])
                                left = median + 1;
                            else if (mDecimal[j] > numbers[median].decimal[j])
                                right = median - 1;
                            else if (mDecimal[j] > numbers[median].decimal[j])
                                left = median + 1;
                            else if (mDecimal[j] < numbers[median].decimal[j])
                                right = median - 1;
                        break;
                    }
                    j++;
                }

                if (isCompleteDecimal) {
                    isCompleteDecimal = 1;
                    break;
                }
            } else if (numbers[median].integer > mInteger) {
                right = median - 1;
                isCompleteDecimal = 0;
            } else {
                left = median + 1;
                isCompleteDecimal = 0;
            }
        }

        if (isCompleteDecimal)
            printf("%d", median + 1);
        else
            printf("%d", 0);
    }
    return 0;
}