/**
 * @file lib_interface.cpp
 * @author Zherdev
 */

#include "dbscan.h"
#include "utils.h"
#include "kdtree2.hpp"
#include "geometric_partitioning.h"
#include "lib_interface.hpp"

int run_pdsdbscan(
  MPI_Comm     mpi_comm,
  const float* input_vectors,
  int          input_len,
  int          input_dims,
  int          min_pts,
  double       eps,
  int          need_partitioning,
  int*         output_cluster_ids,
  int          output_len)
{
	int rank, nproc;

	MPI_Comm_rank(mpi_comm, &rank);
	MPI_Comm_size(mpi_comm, &nproc);

	if(input_dims <= 0 || input_len <= 0 || min_pts <= 0 || eps <= 0 || input_len != output_len)
	{
		return RESULT_WRONG_INPUT;
	}

	// check if nproc is NOT multiple of TWO
	unsigned int proc_count = nproc;

 	while (((proc_count % 2) == 0) && proc_count > 1) // While x is even and > 1
   		proc_count /= 2;

	if(proc_count != 1)
	{
		return RESULT_BAD_NUMBER_OF_PROCESSES;
	}

	NWUClustering::ClusteringAlgo dbs;

	dbs.set_dbscan_params(eps, min_pts);

	MPI_Barrier(mpi_comm);
	double start = MPI_Wtime();

	if(rank == proc_of_interest) cout << "Reading points from array" << endl;
	dbs.read_from_array(input_vectors, input_len, input_dims);
	if(rank == proc_of_interest) cout << "Reading the input data took " << MPI_Wtime() - start << " seconds [pre_processing]"<< endl;

	MPI_Barrier(mpi_comm);

	if (need_partitioning) {
		start = MPI_Wtime();
		// parttition the data file geometrically: preprocessing step
		start_partitioning(mpi_comm, dbs);
		MPI_Barrier(mpi_comm);
		if(rank == proc_of_interest) cout << "Partitioning the data geometrically took " << MPI_Wtime() - start << " seconds [pre_processing]" << endl;
	}

	// gather extra points that falls within the eps radius from the boundary: preprocessing step
	start = MPI_Wtime();
	get_extra_points(mpi_comm, dbs);
	MPI_Barrier(mpi_comm);
	if(rank == proc_of_interest) cout << "Gathering extra point took " << MPI_Wtime() - start << " seconds [pre_processing]" << endl;

	// build the kdtrees: preprocessing step
	start = MPI_Wtime();
	dbs.build_kdtree();
	dbs.build_kdtree_outer();
	MPI_Barrier(mpi_comm);
	if(rank == proc_of_interest) cout << "Build kdtree took " << MPI_Wtime() - start << " seconds [pre_processing]" << endl;

	if(rank == proc_of_interest) cout << endl;
	//run the DBSCAN algorithm
	start = MPI_Wtime();
  run_dbscan_algo_uf_mpi_interleaved(mpi_comm, dbs);
	MPI_Barrier(mpi_comm);
  if(rank == proc_of_interest) cout << "Parallel DBSCAN (init, local computation, and merging) took " << MPI_Wtime() - start << " seconds "<< endl;
	if(rank == proc_of_interest) cout << endl;

	// assign cluster IDs to points
	start = MPI_Wtime();
	dbs.get_clusters_distributed(mpi_comm);
	if(rank == proc_of_interest) cout << "Assigning cluster IDs to points " << MPI_Wtime() - start << " seconds [post_processing]" << endl;

  start = MPI_Wtime();
  dbs.write_clusters_to_array(output_cluster_ids);
  if(rank == proc_of_interest) cout << "Writing clusterIDs " << MPI_Wtime() - start << " seconds [pre_processing]"<< endl;

	return RESULT_OK;
}
