#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_NAME_LENGTH 50

int main(int argc, char *argv[]) {
    int rank, size;
    char processor_name[MAX_NAME_LENGTH];
    int name_len;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &name_len);

    // Get the start time
    start_time = MPI_Wtime();

    // Master node
    if (rank == 0) {
        // Send message to each slave node
        for (int i = 1; i < size; i++) {
            char message[MAX_NAME_LENGTH + 100]; // Max length of message
            sprintf(message, "[MPI_Wtime(): %f ] Hi rank %d, I'm YAH-SHU-KAI from Parallel Programming Design Course in 2024", MPI_Wtime() - start_time, i);
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

        // Receive acknowledgment messages from slave nodes
        char acknowledgment[MAX_NAME_LENGTH + 50];
        for (int i = 1; i < size; i++) {
            MPI_Recv(acknowledgment, MAX_NAME_LENGTH + 50, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("[MPI_Wtime(): %f ] Rank 0 got message from rank %d: %s\n", MPI_Wtime() - start_time, i, acknowledgment); // Print the acknowledgment
        }

        end_time = MPI_Wtime();
        printf("[MPI_Wtime(): %f ] Total time taken: %f seconds\n", end_time - start_time, end_time - start_time);
    }
    // Slave nodes
    else {
        // Receive message from master
        char received_message[MAX_NAME_LENGTH + 100];
        MPI_Recv(received_message, MAX_NAME_LENGTH + 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[MPI_Wtime(): %f ] %s\n", MPI_Wtime() - start_time, received_message);

        // Send acknowledgment message to master
        char acknowledgment[MAX_NAME_LENGTH + 50];
        sprintf(acknowledgment, "Rank %d received. Thank you.", rank);
        MPI_Send(acknowledgment, strlen(acknowledgment) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
