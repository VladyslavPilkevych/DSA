#include <stdio.h>
#include <stdlib.h>

int isPrime(unsigned long long number) {
    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) return 0;
    }
    return 1;
}

int main() {
    unsigned long long first, last;
    scanf("%llu %llu", &first, &last);
    
    unsigned long long firstPrime = (first <= 2) ? 2 : first;
    printf("%llu", firstPrime);

    for (unsigned long long i = firstPrime + 1; i <= last; i ++) {
        if (isPrime(i)) {
            printf(", %llu", i);
        }
    }
    
    return 0;
}
