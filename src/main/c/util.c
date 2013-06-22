#include <mpi.h>
#include <stdio.h>

void MPI_Error_handler(int mpi_e, char *msg) {
  int mpi_e_msg_s;
  char mpi_e_msg[MPI_MAX_ERROR_STRING];

  if (mpi_e != 0) {
    MPI_Error_string(mpi_e, mpi_e_msg, &mpi_e_msg_s);
    fprintf(stderr, "[ERROR] %s: %s\n", msg, mpi_e_msg);
  }
}
