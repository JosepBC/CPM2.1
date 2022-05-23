#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define N 10000000
#define SEQUENTIAL_PART 50000

int prime_numbers[N];
int calc_prime_numbers[2 * N];
int local_num_prime_stored;


void print_vector(int *v, int size) {
    printf("Vector: ");
    for (int i = 0; i < size; i++) printf("%i ", v[i]);
    printf("\n");
}

int main(int na,char* arg[]) {
    int i;
    int num_prime_stored, next_prime_candidate;
    int total_proc, jo;

    printf("Nombre de primers a cercar %d\n",N);

    prime_numbers[0] = 2;
    prime_numbers[1] = 3;

    num_prime_stored = 2;
    next_prime_candidate = 5;

    // First 50000 sequential
    while (num_prime_stored < SEQUENTIAL_PART) {
        for (i = 1;  prime_numbers[i] * prime_numbers[i] <= next_prime_candidate; i++) {
            if (next_prime_candidate % prime_numbers[i] == 0) break;
        }

        if (prime_numbers[i] * prime_numbers[i] > next_prime_candidate) prime_numbers[num_prime_stored++] = next_prime_candidate;
        next_prime_candidate += 2;
    }
    
    int max_prime_candidate = N * log(N) + N * log(log(N));


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &total_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &jo);
    MPI_Status status;
    
    int porcio = (max_prime_candidate - (prime_numbers[SEQUENTIAL_PART - 1] + 2)) / (total_proc - 1);

    int sep = (2 * N) / (total_proc - 1);

    if (jo != 0) {
        //int *calc_prime_numbers = (int*) malloc(sizeof(int) * porcio);

        local_num_prime_stored = 0;
        int ini_prime_candidate = jo * porcio;
        int end_prime_candidate = (jo + 1) * porcio;

        if (jo == 1) {
            ini_prime_candidate = prime_numbers[SEQUENTIAL_PART - 1] + 2;
            end_prime_candidate = jo * porcio;
        }

        for (int prime_candidate = ini_prime_candidate; prime_candidate < end_prime_candidate; prime_candidate += 2) {

            for (i = 1;  prime_numbers[i] * prime_numbers[i] <= prime_candidate; i++) {
                if (prime_candidate % prime_numbers[i] == 0) break;
            }

            if (prime_numbers[i] * prime_numbers[i] > prime_candidate) { 
                calc_prime_numbers[(jo - 1) * sep + local_num_prime_stored] = prime_candidate;
                local_num_prime_stored++;
            }
        }
        printf("Pre send\n");
        MPI_Send(&local_num_prime_stored, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Send(calc_prime_numbers, porcio, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Post send\n");


        //free(calc_prime_numbers);
    } else {
        //int *calc_prime_numbers = (int*) malloc(sizeof(int) * porcio);
        if(calc_prime_numbers == NULL) printf("Malloc error\n");
        printf("Pre recive\n");
        for (int i = 1; i < total_proc; i++){
            MPI_Recv(&local_num_prime_stored, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Going to recive: %d\n", local_num_prime_stored);
            int *calc_prime_numbers = (int*) malloc(sizeof(int) * local_num_prime_stored);

            MPI_Recv(calc_prime_numbers, local_num_prime_stored, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            printf("Status %d\n", status.MPI_ERROR);

            for (int j = 0; j < local_num_prime_stored; j++) {
                if (num_prime_stored >= N) break;
                if (calc_prime_numbers[j] != 0) prime_numbers[num_prime_stored++] = calc_prime_numbers[j];
            }
        }
    }

    
    MPI_Finalize();


    if (prime_numbers[N - 1] == 179424673) {
        printf("Primer ok\n");
        exit(0);
    } else {
        printf("Primer malament\n");
        exit(1);
    }

}