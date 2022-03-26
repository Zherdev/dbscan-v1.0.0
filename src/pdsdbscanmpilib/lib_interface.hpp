/**
 * @file lib_interface.hpp
 * @author Zherdev
 */

#pragma once

#include <mpi.h>

#define RESULT_OK 0
#define RESULT_WRONG_INPUT 1
#define RESULT_BAD_NUMBER_OF_PROCESSES 2

int run_pdsdbscan(
  MPI_Comm     world_comm,
  const float* input_vectors,
  int          input_len,
  int          input_dims,
  int          min_pts,
  double       eps,
  int          need_partitioning,
  int*         output_cluster_ids,
  int          output_len);
