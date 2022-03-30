Modified PDSDBSCAN-D with SWIG Python wrapper
==============================================

Overview
--------

The DBSCAN clustering is a popular unsupervised learning algorithm in machine learning.
PDSDBSCAN-D - is a parallel DBSCAN based on MPI.

This project is a part of our work on parallelizing of the DBSCAN algorithm
in the ClusterLogs framework. Check this paper: *Parallelizing of the DBSCAN
algorithm in the ClusterLogs framework, International Journal of Modern Physics A,
Vol. 37, No. 01, 2150247 (2022); Ivan Zherdev, Konstantin Zhukov, Maria Grigorieva
and Sergey Korobkov*. Please cite the publication above if you use this package.

This repo contains modified PDSDBSCAN-D:
* Implemented a library-like interface. So now PDSDBSCAN can be called as a function.
* Added a SWIG wrapper for integration with Python (numpy + mpi4py).
* Supported the ability of running without the geometrical partitioning.

Usage
-----

```bash
$ git clone git@github.com:Zherdev/pdsdbscan-mpi-lib.git
$ cd pdsdbscan-mpi-lib
$ pip3 install -r requirements.txt
$ pip3 install .
```

```python
from pdsdbscanmpilib import pdsdbscan

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
input_data = np.array([[1,1],[1.1,1.1],[10,10]], dtype=np.float32)
min_pts = 1
eps = 0.5
need_partitioning = 0
output_len = 3

status, result = pdsdbscan.run_pdsdbscan(
  comm, input_data, min_pts, eps, need_partitioning, output_len
)
```
```python
>>> result
array([1, 1, 0], dtype=int32)
```

Requirements
------------
* C++ compiler with C++11 and MPI support
* Python 3
* setuptools, mpi4py and numpy installed

Author
------

Ivan Zherdev, 2022.

Information about the original PDSDBSCAN algorithm can be found in this publication:

```
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*   Description: Implementation of scalable parallel DBSCAN algorithm       */
/*				using the disjoint set data structure        */
/*                                                                           */
/*   Author:  Md. Mostofa Ali Patwary                                        */
/*            EECS Department, Northwestern University                       */
/*            email: mpatwary@eecs.northwestern.edu                          */
/*                                                                           */
/*   Copyright, 2012, Northwestern University                                */
/*   See COPYRIGHT notice in top-level directory.                            */
/*                                                                           */
/*   Please cite the following publication if you use this package 	     */
/* 									     */
/*   Md. Mostofa Ali Patwary, Diana Palsetia, Ankit Agrawal, Wei-keng Liao,  */
/*   Fredrik Manne, and Alok Choudhary, "A New Scalable Parallel DBSCAN      */
/*   Algorithm Using the Disjoint Set Data Structure", Proceedings of the    */
/*   International Conference on High Performance Computing, Networking,     */
/*   Storage and Analysis (Supercomputing, SC'12), pp.62:1-62:11, 2012.	     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
```
