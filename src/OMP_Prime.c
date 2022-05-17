#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

#define N 10000000
#define SEQUENTIAL_PART 50000

int prime_numbers[N];
int calc_prime_numbers[2 * N];

void print_vector(int *v, int size) {
    printf("Vector: ");
    for (int i = 0; i < size; i++) printf("%i ", v[i]);
    printf("\n");
}

int main(int na, char* arg[]) {
    int i = 0;
    int num_prime_stored, next_prime_candidate;

    printf("Nombre de primers a cercar %i\n", N);

    prime_numbers[0] = 2;
    prime_numbers[1] = 3;

    num_prime_stored = 2;
    next_prime_candidate = 5;

    // First milion sequential
    while (num_prime_stored < SEQUENTIAL_PART) {
        for (i = 1;  prime_numbers[i] * prime_numbers[i] <= next_prime_candidate; i++) {
            if (next_prime_candidate % prime_numbers[i] == 0) break;
        }

        if (prime_numbers[i] * prime_numbers[i] > next_prime_candidate) prime_numbers[num_prime_stored++] = next_prime_candidate;
        next_prime_candidate += 2;
    }

    int max_prime_candidate = N * log(N) + N * log(log(N));

    int local_num_prime_stored = 0;

    #pragma omp parallel for private(i) shared(prime_numbers, calc_prime_numbers) firstprivate(local_num_prime_stored)
    for (int prime_candidate = prime_numbers[SEQUENTIAL_PART - 1] + 2; prime_candidate < max_prime_candidate; prime_candidate += 2) {

        for (i = 1;  prime_numbers[i] * prime_numbers[i] <= prime_candidate; i++) {
            if (prime_candidate % prime_numbers[i] == 0) break;
        }

        int jo = omp_get_thread_num();
        int nthread = omp_get_num_threads();
        int porcio = (2 * N) / nthread;

        if (prime_numbers[i] * prime_numbers[i] > prime_candidate) {
            calc_prime_numbers[jo * porcio + local_num_prime_stored] = prime_candidate;
            local_num_prime_stored++;
        }

    }

    for (int elem = 0; elem < 2 * N; elem++) {
        if (num_prime_stored >= N) break;
        if (calc_prime_numbers[elem] != 0) prime_numbers[num_prime_stored++] = calc_prime_numbers[elem];
    }

    if (prime_numbers[N - 1] == 179424673) {
        printf("Primer ok\n");
        exit(0);
    } else {
        printf("Primer malament: %i\n", prime_numbers[N - 1]);
        exit(1);
    }

}