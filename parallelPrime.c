#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define END_MARKER -1

int main(int argc, char** argv) {
    int process_id, total_processes, upper_limit;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

    // Start timer
    double start_time = MPI_Wtime();

    if (process_id == 0) {
        if (argc != 2) {
            printf("Usage: mpirun -np <processes> %s <upper_limit>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        upper_limit = atoi(argv[1]);

        int initial_prime = 2;
        MPI_Send(&initial_prime, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        for (int candidate = 3; candidate <= upper_limit; candidate += 2) { 
            MPI_Send(&candidate, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }

        int end_signal = END_MARKER;
        MPI_Send(&end_signal, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (process_id < total_processes - 1) {
        int current_prime, received_number;
        MPI_Status mpi_status;

        MPI_Recv(&current_prime, 1, MPI_INT, process_id - 1, 0, MPI_COMM_WORLD, &mpi_status);

        while (1) {
            MPI_Recv(&received_number, 1, MPI_INT, process_id - 1, 0, MPI_COMM_WORLD, &mpi_status);
            
            if (received_number == END_MARKER) {
                MPI_Send(&received_number, 1, MPI_INT, process_id + 1, 0, MPI_COMM_WORLD);
                break; 
            }

            if (received_number % current_prime != 0) {
                MPI_Send(&received_number, 1, MPI_INT, process_id + 1, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        int candidate_number;
        MPI_Status mpi_status;

        printf("Prime numbers identified:\n");
        printf("2\n");

        while (1) {
            MPI_Recv(&candidate_number, 1, MPI_INT, process_id - 1, 0, MPI_COMM_WORLD, &mpi_status);
            
            if (candidate_number == END_MARKER) {
                break; 
            }
            
            printf("%d\n", candidate_number);
        }
    }

    // End timer
    double end_time = MPI_Wtime();
    if (process_id == 0) {
        printf("Total execution time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
