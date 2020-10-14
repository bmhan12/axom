#!/bin/bash
##############################################################################
# Copyright (c) 2017-2020, Lawrence Livermore National Security, LLC and
# other Axom Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (BSD-3-Clause)
##############################################################################

set -x

function or_die () {
    "$@"
    local status=$?
    if [[ $status != 0 ]] ; then
        echo ERROR $status command: $@
        exit $status
    fi
}

or_die cd axom
git submodule init 
git submodule update 

echo HOST_CONFIG
echo $HOST_CONFIG

echo "~~~~~~ RUNNING CMAKE ~~~~~~~~"
or_die ./config-build.py -hc /home/axom/axom/host-configs/docker/${HOST_CONFIG}.cmake
or_die cd build-$HOST_CONFIG-debug
echo "~~~~~~ RUNNING make check ~~~~~~~~"
or_die make check

exit 0