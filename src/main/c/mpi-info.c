#include <errno.h>
#include <sys/resource.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>

#include "util.h"


int main(int argc, char **argv) {
  int mpi_e, mpi_s, mpi_r = 0;
  int sum = 0;
  char procname[MPI_MAX_PROCESSOR_NAME];
  struct rlimit limit;

  // ------------------------------------------------------------------------------------------------
  // MPI_Init
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Init(&argc, &argv);
  MPI_Error_handler(mpi_e, "MPI_Init");

  // ------------------------------------------------------------------------------------------------
  // MPI_Comm_size
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Comm_size(MPI_COMM_WORLD, &mpi_s);
  MPI_Error_handler(mpi_e, "MPI_Comm_size");

  // ------------------------------------------------------------------------------------------------
  // MPI_Comm_rank
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_r);
  MPI_Error_handler(mpi_e, "MPI_Comm_rank");

  // ------------------------------------------------------------------------------------------------
  // MPI_Get_processor_name
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Get_processor_name(procname, &sum);
  MPI_Error_handler(mpi_e, "MPI_Get_processor_name");

  // ------------------------------------------------------------------------------------------------
  // basic rank, size and processor output
  // ------------------------------------------------------------------------------------------------

  printf("[INFO] [%s] [%d] MPI_Comm_size(MPI_COMM_WORLD): %d\n", procname, mpi_r, mpi_s);

  // ------------------------------------------------------------------------------------------------
  // resource usage
  // ------------------------------------------------------------------------------------------------

  if (getrlimit(RLIMIT_AS, &limit) == 0) {
    printf("[DEBUG] [%s] [%d] virtual memory limit (RLIMIT_AS): %lu\n", procname, mpi_r, limit.rlim_max);
  } else {
    fprintf(stderr, "[ERROR] [%s] [%d] %d = getrlimit(%d): %s\n", procname, mpi_r, errno, RLIMIT_AS, strerror(errno));
  }

  // ------------------------------------------------------------------------------------------------
  // MPI_Allreduce
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Allreduce(&mpi_r, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Error_handler(mpi_e, "MPI_Allreduce");

  printf("[INFO] [%s] [%d] MPI_Allreduce (sum of all ranks): %d\n", procname, mpi_r, sum);

  // ------------------------------------------------------------------------------------------------
  // MPI_Finalize
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Finalize();
  MPI_Error_handler(mpi_e, "MPI_Finalize");

  return mpi_e;
}
