#include <stdlib.h>
#include <stdio.h>

#define N 10000000

int prime_numbers[N];

int main(int na,char* arg[]) {
    int i;
    int num_prime_stored,next_prime_candidate;

    printf("Nombre de primers a cercar %d\n",N);

    prime_numbers[0] = 2;
    prime_numbers[1] = 3;

    num_prime_stored = 2;
    next_prime_candidate = 5;

    while (num_prime_stored < N) {
        for (i = 1; prime_numbers[i] * prime_numbers[i] <= next_prime_candidate; i++)
            if (next_prime_candidate % prime_numbers[i] == 0) break;
        if (prime_numbers[i] * prime_numbers[i] > next_prime_candidate) prime_numbers[num_prime_stored++] = next_prime_candidate;
        next_prime_candidate += 2;
    }

    if (prime_numbers[N - 1] == 179424673) {
        printf("Primer ok\n");
        exit(0);
    } else {
        printf("Primer malament\n");
        exit(1);
    }

}
© 2022 GitHub, Inc.
Terms
