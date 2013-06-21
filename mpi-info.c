#include <errno.h>
#include <sys/resource.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int mpi_e, mpi_e_msg_s, mpi_s, mpi_r = 0;
  int sum = 0;
  char procname[MPI_MAX_PROCESSOR_NAME];
  char mpi_e_msg[MPI_MAX_ERROR_STRING];
  struct rlimit limit;

  // ------------------------------------------------------------------------------------------------
  // MPI_Init
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Init(&argc, &argv);

  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] MPI_Init(%d, %s): %s\n", argc, *argv, mpi_e_msg);
  }

  // ------------------------------------------------------------------------------------------------
  // MPI_Comm_size
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Comm_size(MPI_COMM_WORLD, &mpi_s);

  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] MPI_Comm_size(MPI_COMM_WORLD): %s\n", mpi_e_msg);
  }

  // ------------------------------------------------------------------------------------------------
  // MPI_Comm_rank
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_r);

  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] MPI_Comm_rank(MPI_COMM_WORLD): %s\n", mpi_e_msg);
  }

  // ------------------------------------------------------------------------------------------------
  // MPI_Get_processor_name
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Get_processor_name(procname, &sum);
  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] MPI_Get_processor_name(): %s\n", mpi_e_msg);
  }

  // ------------------------------------------------------------------------------------------------
  // basic rank, size and processor output
  // ------------------------------------------------------------------------------------------------

  printf("[INFO] [%s] [%d] MPI_Comm_size(MPI_COMM_WORLD): %d\n", procname, mpi_r, mpi_s);

  // ------------------------------------------------------------------------------------------------
  // resource usage
  // ------------------------------------------------------------------------------------------------

  if (getrlimit(RLIMIT_AS, &limit) == 0) {
    printf("[INFO] [%s] [%d] virtual memory limit (RLIMIT_AS): %lu\n", procname, mpi_r, limit.rlim_max);
  } else {
    printf("[INFO] [%s] [%d] %d = getrlimit(%d): %s\n", procname, mpi_r, errno, RLIMIT_AS, strerror(errno));
  }

  // ------------------------------------------------------------------------------------------------
  // MPI_Allreduce
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Allreduce(&mpi_r, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] MPI_Allreduce(%d, sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD): %s\n", mpi_r, mpi_e_msg);
  }

  printf("[INFO] [%s] [%d] MPI_Allreduce (sum of all ranks): %d\n", procname, mpi_r, sum);

  // ------------------------------------------------------------------------------------------------
  // MPI_Finalize
  // ------------------------------------------------------------------------------------------------

  mpi_e = MPI_Finalize();

  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] MPI_Finalize: %s\n", mpi_e_msg);
  }

  return mpi_e;
}
