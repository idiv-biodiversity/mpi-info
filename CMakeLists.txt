
# -------------------------------------------------------------------------------
# project definition
# -------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

project(mpi-info)

# -------------------------------------------------------------------------------
# sources
# -------------------------------------------------------------------------------

set(SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/src/main/c")

set(MPI_INFO_SRCS
  ${SRCDIR}/mpi-info.c
  ${SRCDIR}/util.c
)

set(MPI_INFO_H
  ${SRCDIR}/util.h
)

# -------------------------------------------------------------------------------
# dependencies
# -------------------------------------------------------------------------------

include(FindMPI)
include_directories(${MPI_C_INCLUDE_PATH})

include(CheckIncludeFile)
check_include_file(errno.h HAVE_ERRNO_H)
check_include_file(sys/resource.h HAVE_SYS_RESOURCE_H)
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(string.h HAVE_STRING_H)

include(CheckSymbolExists)
check_symbol_exists(fprintf stdio.h HAVE_FPRINTF)
check_symbol_exists(getrlimit sys/resource.h HAVE_GETRLIMIT)
check_symbol_exists(printf stdio.h HAVE_PRINTF)
check_symbol_exists(RLIMIT_AS sys/resource.h HAVE_RLIMIT_AS)
check_symbol_exists(stderr stdio.h HAVE_STDERR)
check_symbol_exists(strerror string.h HAVE_STRERROR)

# -------------------------------------------------------------------------------
# end products
# -------------------------------------------------------------------------------

add_executable(mpi-info ${MPI_INFO_SRCS} ${MPI_INFO_H})
target_link_libraries(mpi-info ${MPI_C_LIBRARIES})

# -------------------------------------------------------------------------------
# tests
# -------------------------------------------------------------------------------

enable_testing()
add_test(NAME simple-execution
         COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS}
                 mpi-info)

# -------------------------------------------------------------------------------
# installation
# -------------------------------------------------------------------------------

install (TARGETS mpi-info DESTINATION bin)
