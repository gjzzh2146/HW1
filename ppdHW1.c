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

    // Master node
    if (rank == 0) {
        char message[MAX_NAME_LENGTH + 100]; // Max length of message
        sprintf(message, "Hi rank %d, I'm %s from Parallel Programming Design Course in 2024", rank, processor_name);
        start_time = MPI_Wtime();

        // Send message to each slave node
        for (int i = 1; i < size; i++) {
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

        // Receive messages from slave nodes
        for (int i = 1; i < size; i++) {
            char received_message[MAX_NAME_LENGTH + 100];
            MPI_Recv(received_message, MAX_NAME_LENGTH + 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end_time = MPI_Wtime();
            printf("%s\n", received_message);
        }

        printf("Total time taken: %f seconds\n", end_time - start_time);
    }
    // Slave nodes
    else {
        char received_message[MAX_NAME_LENGTH + 100];
        MPI_Recv(received_message, MAX_NAME_LENGTH + 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%s\n", received_message);

        // Send back acknowledgment message
        char acknowledgment[MAX_NAME_LENGTH + 50];
        sprintf(acknowledgment, "Rank %d received. Thank you.", rank);
        MPI_Send(acknowledgment, strlen(acknowledgment) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
