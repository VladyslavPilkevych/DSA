#include stdlib.h
#include stdio.h

unsigned long long euklidovAlgoritmus(unsigned long long a, unsigned long long b)
{
    unsigned long long buffer = 1;
    while (b != 0) {
        buffer = b;
        b = a % b;
        a = buffer;
    }
    return a;
}

int main()
{
    int linesLength;
    unsigned long long a, b;
    scanf(%d, &linesLength);

    for (int i = linesLength; i  0; --i)
    {
        scanf(%llu %llu, &a, &b);
        if (i - 1 == 0)
            printf(%llu, euklidovAlgoritmus(a, b));
        else
            printf(%llun, euklidovAlgoritmus(a, b));
    }

    return 0;
}
