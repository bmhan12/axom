#!/bin/bash

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

#MSUB -l nodes=1
#MSUB -q psmall
#MSUB -l walltime=8:00:00
#MSUB -A ddcwork
#MSUB -j oe
#MSUB -o m.out.cz.uberenv.bgqos.src.%j.%N.txt
#
# usage: 
#  cd {to directory with this script}
#  msub -d `pwd` msub_llnl_cz_bgq_src.sh
#

date
. /usr/local/tools/dotkit/init.sh
use python-2.7.3
use git-2.0.0
use cmake-3.8.2

cd ..
./build_src.py
date
