#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {

    clock_t startClock, endClock;
    double elapsedTime;
    
    if (argc != 2) {
        printf("Usage: %s <upper_limit>\n", argv[0]);
        return 1;
    }

    startClock = clock();

    int n = atoi(argv[1]);
    printf("Calculating prime numbers up to %d:\n", n);

    int* primeFlags = (int*)malloc((n + 1) * sizeof(int));
    
    for (int i = 0; i <= n; i++) {
        primeFlags[i] = 1;
    }
    
    primeFlags[0] = primeFlags[1] = 0;

    for (int i = 2; i * i <= n; i++) {
        if (primeFlags[i]) {
            for (int multiple = i * i; multiple <= n; multiple += i) {
                primeFlags[multiple] = 0;
            }
        }
    }

    int primeCount = 0;
    printf("Prime numbers: ");
    for (int i = 2; i <= n; i++) {
        if (primeFlags[i]) {
            printf("%d ", i);
            primeCount++;
        }
    }
    printf("\nTotal primes found: %d\n", primeCount);

    endClock = clock();
    elapsedTime = ((double) (endClock - startClock)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f seconds\n", elapsedTime);

    free(primeFlags);
    return 0;
}
