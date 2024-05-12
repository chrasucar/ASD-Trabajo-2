#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int num_procs, rank;
    double inicio = 0.0, final = 0.0, total = 0.0;
    double step, pi, aux;
    double x, sum = 0.0;
    long num_steps = 500000;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    step = 1.0 / (double)num_steps;

    inicio = MPI_Wtime();

    long start = rank * (num_steps / num_procs);
    long end = (rank + 1) * (num_steps / num_procs);
    if (rank == num_procs - 1) {
        end = num_steps;
    }

    for (long i = start; i < end; i++) {
        x = (i + 0.5) * step;
        aux = 4.0 / (1.0 + x * x);
        sum += aux;
    }

    double local_pi = step * sum;
    double total_pi;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&local_pi, &total_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    final = MPI_Wtime();
    total = final - inicio;

    if (rank == 0) {
    	pi = total_pi;
        printf("El resultado es pi : %.5f\n", pi);
        printf("Tiempo de ejecucion: %.8f\n", total);
        printf("Numero de procesos lanzados: %i\n", num_procs);
    }

    MPI_Finalize();
    return 0;
}
