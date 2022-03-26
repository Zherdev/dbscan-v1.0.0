#! /usr/bin/env python

from setuptools import Extension, find_packages, setup
from setuptools.command.build_py import build_py

import numpy
import mpi4py
import os

try:
  numpy_include = numpy.get_include()
except AttributeError:
  numpy_include = numpy.get_numpy_include()
try:
  mpi4py_include = mpi4py.get_include()
except AttributeError:
  mpi4py_include = mpi4py.get_numpy_include()


mpi_compile_args = os.popen("mpic++ --showme:compile").read().strip().split(' ')
mpi_link_args    = os.popen("mpic++ --showme:link").read().strip().split(' ')

_pdsdbscan = Extension(
  "_pdsdbscan",
  [
    "src/pdsdbscanmpilib/pdsdbscan.i",
    "src/pdsdbscanmpilib/lib_interface.cpp",
    "src/pdsdbscanmpilib/clusters.cpp",
    "src/pdsdbscanmpilib/dbscan.cpp",
    "src/pdsdbscanmpilib/geometric_partitioning.cpp",
    "src/pdsdbscanmpilib/kdtree2.cpp",
    "src/pdsdbscanmpilib/utils.cpp",
  ],
  swig_opts=['-c++'],
  include_dirs = [
    "/usr/local/include",
    numpy_include,
    mpi4py_include,
    "src/pdsdbscanmpilib",
  ],
  language = 'c++',
  extra_compile_args = mpi_compile_args,
  extra_link_args = mpi_link_args,
)

class BuildPy(build_py):
  def run(self):
    self.run_command('build_ext')
    super(build_py, self).run()

setup(
  name         = "pdsdbscanmpilib",
  description  = "Parallel distjoint set dbscan swig-wrapper for Python",
  author       = "Ivan Zherdev",
  author_email = "zherdevivanp@gmail.com",
  license      = "Northwestern University License",
  version      = "1.0.0",
  ext_modules  = [_pdsdbscan],
  packages     = find_packages('src'),
  package_dir  = {'': 'src'},
  package_data = {'pdsdbscanmpilib': ['*.so']},
  cmdclass     = {
      'build_py': BuildPy,
  },
)
