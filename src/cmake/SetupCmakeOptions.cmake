# Please add configuration options here.
# This provides a central location to check options and their defaults and ensure
# these are declared at the top of the cmake configuration.

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(BUILD_TESTING "Builds unit tests" ON)
option(ENABLE_CODECOV "Enable code coverage via gcov." OFF)
option(ENABLE_CXX11 "Enables C++11 language support." ON)
option(ENABLE_BOOST "Enable Boost" OFF)
option(ENABLE_FORTRAN "Enables Fortran compiler support." ON)
option(ENABLE_GLOBALCOMPILERWARNINGS "Enable extra compiler warnings on all build targets." ON)
option(ENABLE_GLOBALCOMPILERWARNINGSASERRORS "Enable treating compiler warnings as errors on all build targets." OFF)
option(ENABLE_HDF5"ENABLE HDF5" ON)
option(ENABLE_MPI "Enables MPI compiler support" OFF)
option(ENABLE_OPENMP "Enables OpenMP compiler support" OFF)
option(ENABLE_BENCHMARK "Enables google benchmark" OFF)

