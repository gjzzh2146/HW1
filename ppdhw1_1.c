#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char message[100];
    sprintf(message, "Hi rank %d, I'm YAH-SHU-KAI from Parallel Programming Design Course in 2024", rank);

    double start_time = MPI_Wtime();

    if (rank == 0) {
        // Master node
        for (int i = 1; i < size; i++) {
            sprintf(message, "[MPI_Wtime(): %f] %s", MPI_Wtime(), message);
            MPI_Send(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Slave nodes
        MPI_Recv(message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[MPI_Wtime(): %f] Rank %d got message from rank 0: %s\n", MPI_Wtime(), rank, message);
    }

    double end_time = MPI_Wtime();

    MPI_Finalize();

    return 0;
}
