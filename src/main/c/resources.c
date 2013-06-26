#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <string.h>
#include <unistd.h>

void RES_Memory_test() {
  int mpi_r, resultlen = 0;
  char procname[MPI_MAX_PROCESSOR_NAME];
  size_t base = sizeof(size_t);
  size_t bytes = 1073741824;
  size_t size = bytes / base;
  size_t *blob;
  struct rlimit limit;

  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_r);
  MPI_Get_processor_name(procname, &resultlen);

  if (getrlimit(RLIMIT_AS, &limit) == 0) {
    printf("%s %d %d virtual memory limit (RLIMIT_AS): %lu\n", procname, getpid(), mpi_r, limit.rlim_max);
  } else {
    fprintf(stderr, "%s %d %d getrlimit(%d): %s\n", procname, getpid(), mpi_r, RLIMIT_AS, strerror(errno));
  }

  errno = 0;
  blob = (size_t*)malloc(base * size);

  if (errno != 0 || blob == NULL) {
    fprintf(stderr, "%s %d %d malloc error %d: %s\n", procname, getpid(), mpi_r, errno, strerror(errno));
  } else {
    size_t i,j = 0;

    for (i = 0; i < size; i++) {
      blob[i] = 42;
      j++;
    }

    printf("%s %d %d iterations (planned/done) %lu/%lu\n", procname, getpid(), mpi_r, i, j);
  }

  free(blob);
}
