/* Algoritmo MonteCarlo*/

// Librerías

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// Definir Master: 0

#define MASTER 0

int main(int argc, char* argv[]) {

    int rank, size;
    unsigned long long local_count = 0;
    long long i;
    long long local_samples, total_samples;
    double x, y;
    double start, end;

    // Iniciar MPI

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Valor por defecto */

    total_samples = 3000000;

    /* Para su uso en línea de comandos */

    if (argc > 1)

        total_samples = atoll(argv[1]);

    local_samples = total_samples / size;

    // Semilla diferente para cada proceso

    srand(time(NULL) + rank); 

    // Todos los procesos comienzan al mismo tiempo

    MPI_Barrier(MPI_COMM_WORLD); 

    start = MPI_Wtime(); 

    for (i = 0; i < local_samples; ++i) {

        x = ((double)rand()) / ((double)RAND_MAX); // 0 ≤ x ≤ 1
        y = ((double)rand()) / ((double)RAND_MAX);

        if (x * x + y * y <= 1.0) {

            ++local_count;

        }
    }

    unsigned long long* global_counts = NULL;

    if (rank == MASTER) {

        global_counts = (unsigned long long*) malloc (size * sizeof(unsigned long long));

    }

    MPI_Gather(&local_count, 1, MPI_UNSIGNED_LONG_LONG, global_counts, 1, MPI_UNSIGNED_LONG_LONG, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        unsigned long long global_count = 0;
        for (i = 0; i < size; ++i) {
            global_count += global_counts[i];
        }
        double end = MPI_Wtime(); // Finalizamos la medición del tiempo
        double time_taken = end - start;

        printf("\nValor estimado de pi: %.7f\n", 4.0 * global_count / total_samples);
        printf("Tiempo de ejecución: %.5f segundos\n", time_taken);

        free(global_counts);
    }

    MPI_Finalize();

    return 0;
}
