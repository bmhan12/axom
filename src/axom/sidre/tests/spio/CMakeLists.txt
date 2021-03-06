# Copyright (c) 2017-2020, Lawrence Livermore National Security, LLC and
# other Axom Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (BSD-3-Clause)
#------------------------------------------------------------------------------
# Spio unit tests
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Specify list of tests
#------------------------------------------------------------------------------

set(gtest_spio_serial_tests
   spio_serial.hpp
   )

set(gtest_spio_parallel_tests
   spio_basic.hpp
   spio_parallel.hpp
   )

set(spio_test_depends
    sidre
    conduit::conduit
    mpi
    )

blt_list_append(TO spio_test_depends ELEMENTS hdf5 IF HDF5_FOUND)

if(SCR_FOUND)
    list(APPEND spio_test_depends scr)
    list(APPEND gtest_spio_parallel_tests spio_scr.hpp)
endif()

#------------------------------------------------------------------------------
# Add gtest C++ tests
#------------------------------------------------------------------------------
blt_add_executable( NAME       spio_tests
                    SOURCES    spio_main.cpp
                    HEADERS    ${gtest_spio_parallel_tests} ${gtest_spio_serial_tests}
                    OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                    DEPENDS_ON ${spio_test_depends} gtest
                    FOLDER     axom/sidre/tests
                    )

foreach(test ${gtest_spio_serial_tests})
    get_filename_component( test_name ${test} NAME_WE )
    blt_add_test( NAME          ${test_name} 
                  COMMAND       spio_tests --gtest_filter=${test_name}*
                  NUM_MPI_TASKS 1
                 )
endforeach()

foreach(test ${gtest_spio_parallel_tests})
    get_filename_component( test_name ${test} NAME_WE )
    blt_add_test( NAME          ${test_name} 
                  COMMAND       spio_tests --gtest_filter=${test_name}*
                  NUM_MPI_TASKS 4
                  )
endforeach()

#------------------------------------------------------------------------------
# Add Fortran tests
#------------------------------------------------------------------------------
if(ENABLE_FORTRAN)
    # Non-Fruit parallel tests with 1 MPI task
    set(spio_one_fortran_tests
        F_spio_basicWriteRead.F
        F_spio_blueprintIndex.F
        )

    foreach(test ${spio_one_fortran_tests})
        get_filename_component( test_name ${test} NAME_WE )
        string(SUBSTRING "${test_name}" 2 -1 test_name) # remove F_
        blt_add_executable( NAME       ${test_name}_F_test
                            SOURCES    ${test}
                            OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                            DEPENDS_ON ${spio_test_depends}
                            FOLDER     axom/sidre/tests
                            )
        blt_add_test( NAME    ${test_name}_F
                      COMMAND ${test_name}_F_test
                      NUM_MPI_TASKS 1
                      )
    endforeach()

    # Non-Fruit parallel tests with 4 MPI task
    set(spio_four_fortran_tests
        F_spio_parallelWriteRead.F
        F_spio_irregularWriteRead.F
        F_spio_externalWriteRead.F
        )

    foreach(test ${spio_four_fortran_tests})
        get_filename_component( test_name ${test} NAME_WE )
        string(SUBSTRING "${test_name}" 2 -1 test_name) # remove F_
        blt_add_executable( NAME       ${test_name}_F_test
                            SOURCES    ${test}
                            OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                            DEPENDS_ON ${spio_test_depends}
                            FOLDER     axom/sidre/tests
                            )
        blt_add_test( NAME    ${test_name}_F
                      COMMAND ${test_name}_F_test
                      NUM_MPI_TASKS 4
                      )
    endforeach()

    # Fruit parallel tests with 4 MPI task
    set(spio_fruit_tests
        F_spio_preserveWriteRead.F
        )

    foreach(test ${spio_fruit_tests})
        get_filename_component( test_name ${test} NAME_WE )
        string(SUBSTRING "${test_name}" 2 -1 test_name) # remove F_
        blt_add_executable( NAME       ${test_name}_F_test
                            SOURCES    ${test}
                            OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                            DEPENDS_ON ${spio_test_depends} fruit
                            FOLDER     axom/sidre/tests
                            )
        blt_add_test( NAME    ${test_name}_F
                      COMMAND ${test_name}_F_test
                      NUM_MPI_TASKS 4
                      )
    endforeach()

endif()
