##################################
# uberenv host-config
#
# This is a generated file, edit at own risk.
##################################
# chaos_5_x86_64_ib-intel@17.0.174
##################################

# cmake from uberenv
# cmake executable path: /usr/workspace/wsa/axom/thirdparty_libs/builds/2018_05_04_12_25_12/spack/opt/spack/chaos_5_x86_64_ib/intel-17.0.174/cmake-3.8.2-j4f2uiw4gpt5acb6f4p7rjn6jvpbue5i/bin/cmake

#######
# using intel@17.0.174 compiler spec
#######

# c compiler used by spack
set(CMAKE_C_COMPILER "/usr/local/tools/ic-17.0.174/bin/icc" CACHE PATH "")

# cpp compiler used by spack
set(CMAKE_CXX_COMPILER "/usr/local/tools/ic-17.0.174/bin/icpc" CACHE PATH "")

# fortran compiler used by spack
set(ENABLE_FORTRAN ON CACHE BOOL "")

set(CMAKE_Fortran_COMPILER "/usr/local/tools/ic-17.0.174/bin/ifort" CACHE PATH "")

# Root directory for generated TPLs
set(TPL_ROOT "/usr/workspace/wsa/axom/thirdparty_libs/builds/2018_05_04_12_25_12/spack/opt/spack/chaos_5_x86_64_ib/intel-17.0.174" CACHE PATH "")

# hdf5 from uberenv
set(HDF5_DIR "${TPL_ROOT}/hdf5-1.8.16-yle5lfpy32ffwzm54w2btci2rwgxpnj5" CACHE PATH "")

# scr not built by uberenv

# conduit from uberenv
set(CONDUIT_DIR "${TPL_ROOT}/conduit-0.3.1-ownyq4n6cwxyfry3c3ajz3thvgzbvaqw" CACHE PATH "")

# mfem from uberenv
set(MFEM_DIR "${TPL_ROOT}/mfem-3.3.2-hyb5naavqgv7cqibw7lozrgom3ynczek" CACHE PATH "")

# python from uberenv
set(PYTHON_EXECUTABLE "${TPL_ROOT}/python-2.7.11-gsatbg7hsqa6y57pmds3ysr6zhhxmleh/bin/python" CACHE PATH "")

# lua from uberenv
set(LUA_DIR "${TPL_ROOT}/lua-5.1.5-mne43sd5phd4m5qp23urcngva3xzjfq2" CACHE PATH "")

# doxygen from uberenv
set(DOXYGEN_EXECUTABLE "${TPL_ROOT}/doxygen-1.8.11-33gzcqdym2leojxte3y235y35xf6wmrd/bin/doxygen" CACHE PATH "")

# sphinx 1.4.5 from uberenv
set(SPHINX_EXECUTABLE "${TPL_ROOT}/python-2.7.11-gsatbg7hsqa6y57pmds3ysr6zhhxmleh/bin/sphinx-build" CACHE PATH "")

# shroud 0.9.0 from uberenv
set(SHROUD_EXECUTABLE "${TPL_ROOT}/python-2.7.11-gsatbg7hsqa6y57pmds3ysr6zhhxmleh/bin/shroud" CACHE PATH "")

# uncrustify from uberenv
set(UNCRUSTIFY_EXECUTABLE "${TPL_ROOT}/uncrustify-0.61-4mhnkbt4d4vjxremrvvyknidkdqtud5o/bin/uncrustify" CACHE PATH "")

# lcov and genhtml from uberenv
set(LCOV_PATH "${TPL_ROOT}/lcov-1.11-4bjbbbg3jjp5apf3qcvbwhxspyfjrkmv/usr/bin/lcov" CACHE PATH "")

set(GENHTML_PATH "${TPL_ROOT}/lcov-1.11-4bjbbbg3jjp5apf3qcvbwhxspyfjrkmv/usr/bin/genhtml" CACHE PATH "")

##################################
# end uberenv host-config
##################################

##
## Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
##
## Produced at the Lawrence Livermore National Laboratory.
##
## LLNL-CODE-741217
##
## All rights reserved.
##
## This file is part of Axom.
##
## For details about use and distribution, please read axom/LICENSE.
##

##############################################################################
# !---------------------------------------------------------------------------
##############################################################################
# Options added manually to 
# lc chaos5 intel@17.0.174 host configs
##############################################################################

set(ENABLE_GTEST_DEATH_TESTS ON CACHE BOOL "")

# Set flags for intel to use a gcc standard library with C++11
set(GNU_PREFIX           "/usr/apps/gnu/4.9.3")
set(BLT_C_FLAGS          "-gnu-prefix=${GNU_PREFIX}/bin/" CACHE STRING "")
set(BLT_CXX_FLAGS        "-gnu-prefix=${GNU_PREFIX}/bin/" CACHE STRING "")
set(BLT_FORTRAN_FLAGS    "-gnu-prefix=${GNU_PREFIX}/bin/" CACHE STRING "")
set(BLT_EXE_LINKER_FLAGS "-Wl,-rpath,${GNU_PREFIX}/lib64" CACHE STRING "")

##############################################################################
# MPI - manually added for now
##############################################################################
set(ENABLE_MPI ON CACHE BOOL "")

set(MPI_HOME             "/usr/local/tools/mvapich2-intel-2.0" CACHE PATH "")
set(MPI_C_COMPILER       "${MPI_HOME}/bin/mpicc" CACHE PATH "")
set(MPI_CXX_COMPILER     "${MPI_HOME}/bin/mpicxx" CACHE PATH "")
set(MPI_Fortran_COMPILER "${MPI_HOME}/bin/mpif90" CACHE PATH "")

set(MPIEXEC              "/usr/bin/srun" CACHE PATH "")
set(MPIEXEC_NUMPROC_FLAG "-n" CACHE PATH "")

##############################################################################
# !---------------------------------------------------------------------------
##############################################################################

