#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define N 10000000
#define SEQ_PERCENT 5

int prime_numbers[2 * N];


int main(int nargs, char* args[]) {
    int num_prime_stored, next_prime_candidate;
    int total_proc, jo;

    MPI_Init(&nargs, &args);
    MPI_Comm_size(MPI_COMM_WORLD, &total_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &jo);
    
    if (jo == 1) printf("Nombre de primers a cercar %d\n", N);

    // First 50000 sequential in all threads, communication is more expensive
    prime_numbers[0] = 2;
    prime_numbers[1] = 3;

    num_prime_stored = 2;
    next_prime_candidate = 5;

    // Calc max number to check if it's prime
    int max_prime_candidate = N * log(N) + N * log(log(N));

    int end_prime_candidate = max_prime_candidate * (float)SEQ_PERCENT / (float)N;

    if ((float)SEQ_PERCENT / (float)N < 0.01) end_prime_candidate = max_prime_candidate * 0.01;

    for (int prime_candidate = 5; prime_candidate < end_prime_candidate; prime_candidate += 2) {
        int i;
        for (i = 1;  prime_numbers[i] * prime_numbers[i] <= prime_candidate; i++) {
            if (prime_candidate % prime_numbers[i] == 0) break;
        }

        if (prime_numbers[i] * prime_numbers[i] > prime_candidate) { 
            prime_numbers[num_prime_stored++] = prime_candidate;
        }
    }

    int local_num_prime_stored = num_prime_stored;

    // Calc first and last prime candidate
    int ini_prime_candidate = end_prime_candidate + (jo * ((max_prime_candidate - end_prime_candidate) / total_proc));

    if (jo == 0) ini_prime_candidate = end_prime_candidate;
    if (ini_prime_candidate % 2 == 0) ini_prime_candidate--;

    end_prime_candidate = end_prime_candidate + ((jo + 1) * ((max_prime_candidate - end_prime_candidate) / total_proc));
    if (end_prime_candidate % 2 == 0) end_prime_candidate--;

    // Calc parallel primes
    for (int prime_candidate = ini_prime_candidate; prime_candidate < end_prime_candidate; prime_candidate += 2) {

        int i;
        for (i = 1;  prime_numbers[i] * prime_numbers[i] <= prime_candidate; i++) {
            if (prime_candidate % prime_numbers[i] == 0) break;
        }

        if (prime_numbers[i] * prime_numbers[i] > prime_candidate) { 
            prime_numbers[local_num_prime_stored++] = prime_candidate;
        }
    }

    // Index of global vector per thread
    int global_vector_indexs[total_proc];
    // Number of elements calculated per thread
    int num_elem_thread[total_proc];

    if (jo == 0) {
        int num_primes_calculated;

        global_vector_indexs[0] = 0; //Index of root is 0, it already has the sequential and it's paralel slice calculated primes 
        num_elem_thread[0] = local_num_prime_stored; //Number of calculated elements by root

        for (int i = 1; i < total_proc; i++) {
            MPI_Recv(&num_primes_calculated, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            num_elem_thread[i] = num_primes_calculated;
            global_vector_indexs[i] = global_vector_indexs[i - 1] + num_elem_thread[i - 1];
        }

    } else {
        // Send num primes calculated to root process
        int num_primes_calculated = local_num_prime_stored - num_prime_stored;
        MPI_Ssend(&num_primes_calculated, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Gatherv(&prime_numbers[num_prime_stored], local_num_prime_stored - num_prime_stored, MPI_INT,
                &prime_numbers, num_elem_thread, global_vector_indexs, MPI_INT, 0, MPI_COMM_WORLD);

    if (jo == 0) {
        if (prime_numbers[N - 1] == 179424673) {
            printf("Primer ok\n");
        } else {
            printf("Primer malament: %i\n", prime_numbers[N - 1]);
            MPI_Finalize();
            exit(1);
        }
    }

    MPI_Finalize();
    return 0;

}