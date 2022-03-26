%module pdsdbscan

%{
  #define SWIG_FILE_WITH_INIT
  #include <mpi.h>
  #include "lib_interface.hpp"
%}

%include "mpi4py.i"
%mpi4py_typemap(Comm, MPI_Comm);

%include "numpy.i"

%init %{
  import_array();
%}

%apply (float* INPLACE_ARRAY2, int DIM1, int DIM2) {(const float* input_vectors, int input_len, int input_dims)}
%apply (int* ARGOUT_ARRAY1, int DIM1) {(int* output_cluster_ids, int output_len)}

%include "lib_interface.hpp"
