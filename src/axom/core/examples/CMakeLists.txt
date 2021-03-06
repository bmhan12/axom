# Copyright (c) 2017-2020, Lawrence Livermore National Security, LLC and
# other Axom Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (BSD-3-Clause)
#------------------------------------------------------------------------------
# Sidre and Spio examples
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# List of single source file examples
#------------------------------------------------------------------------------
set(example_sources
    core_numerics.cpp
    core_containers.cpp )

set(sidre_example_depends
    core )

#------------------------------------------------------------------------------
# Add targets and tests for serial examples
#------------------------------------------------------------------------------
foreach(example_source ${example_sources})
    get_filename_component(exe_name ${example_source} NAME_WE)
    blt_add_executable(
        NAME        ${exe_name}_ex
        SOURCES     ${example_source}
        OUTPUT_DIR  ${EXAMPLE_OUTPUT_DIRECTORY}
        DEPENDS_ON  ${sidre_example_depends}
        FOLDER      axom/core/examples )

    if(AXOM_ENABLE_TESTS)
        blt_add_test(
            NAME    ${exe_name}
            COMMAND ${exe_name}_ex )
    endif()
endforeach()

blt_add_executable(
    NAME        core_utilities_ex
    SOURCES     core_utilities.cpp
    OUTPUT_DIR  ${EXAMPLE_OUTPUT_DIRECTORY}
    DEPENDS_ON  ${sidre_example_depends}
    FOLDER      axom/core/examples )

if(AXOM_ENABLE_TESTS)
    blt_add_test(
        NAME    core_utilities
        COMMAND core_utilities_ex ${CMAKE_CURRENT_SOURCE_DIR} )
endif()
