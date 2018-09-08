#------------------------------------------------------------------------------
# Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
#
# Produced at the Lawrence Livermore National Laboratory
#
# LLNL-CODE-741217
#
# All rights reserved.
#
# This file is part of Axom.
#
# For details about use and distribution, please read axom/LICENSE.
#------------------------------------------------------------------------------

#
# Specify list of tests
#

set(gtest_lumberjack_tests
    lumberjack_Lumberjack.cpp
    lumberjack_Message.cpp
    lumberjack_TextEqualityCombiner.cpp
    lumberjack_RootCommunicator.cpp
    )

#
# Add gtest based tests
#
foreach(test ${gtest_lumberjack_tests})
    get_filename_component( test_name ${test} NAME_WE )
    blt_add_executable( NAME ${test_name}_test
                        SOURCES ${test}
                        OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                        DEPENDS_ON lumberjack gtest 
                        FOLDER axom/lumberjack/tests )
    blt_add_test( NAME ${test_name} 
                  COMMAND ${test_name}_test )
endforeach()

blt_add_executable(NAME lumberjack_speed_test
                   SOURCES lumberjack_speedTest.cpp
                   OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                   DEPENDS_ON lumberjack
                   FOLDER axom/lumberjack/tests )

blt_add_test( NAME lumberjack_speedTest_binary
              COMMAND lumberjack_speed_test b 10 ${lumberjack_SOURCE_DIR}/tests/sampleInput/loremIpsum02
              NUM_MPI_TASKS 4)
              
blt_add_test( NAME lumberjack_speedTest_root
              COMMAND lumberjack_speed_test r 10 ${lumberjack_SOURCE_DIR}/tests/sampleInput/loremIpsum02
              NUM_MPI_TASKS 4)
              