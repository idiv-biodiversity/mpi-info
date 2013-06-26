#include <mpi.h>
#include "communication.h"
#include "resources.h"

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  MPI_Communication_test();
  RES_Memory_test();

  return MPI_Finalize();
}
