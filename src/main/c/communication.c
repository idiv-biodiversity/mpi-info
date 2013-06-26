#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

void MPI_Communication_test() {
  int mpi_r, sum, resultlen = 0;
  char procname[MPI_MAX_PROCESSOR_NAME];

  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_r);
  MPI_Get_processor_name(procname, &resultlen);

  MPI_Allreduce(&mpi_r, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  printf("%s %d %d MPI_Allreduce (sum of all ranks): %d\n", procname, getpid(), mpi_r, sum);
}
