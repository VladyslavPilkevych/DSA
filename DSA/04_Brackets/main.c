#include <stdlib.h>
#include <stdio.h>

void main(){
    int n, arrayCounter, errorCounter;
    char c;
    char *array = (char *)malloc(512 * sizeof(char));

    scanf("%d ", &n);

    for (int i = 0; i < n; i++)
    {
        arrayCounter = 0;
        errorCounter = 0;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            if (c == '(' || c == '{' || c == '[' || c == '<') array[arrayCounter++] = c;
            else if ((array[arrayCounter-1] == '(' && c == ')')
                        || (array[arrayCounter-1] == '{' && c == '}')
                        || (array[arrayCounter-1] == '[' && c == ']')
                        || (array[arrayCounter-1] == '<' && c == '>'))
                arrayCounter--;
            else
            {
                errorCounter++;
                if (arrayCounter != 0)
                    arrayCounter--;
            }
        }
        errorCounter += arrayCounter;
        printf("%d", errorCounter);
        if (i != n-1) printf("\n");
    }
    free(array);
}
