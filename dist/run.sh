#!/bin/bash

set -e

ulimit -c unlimited

RELATIVE_DIR=`dirname "$0"`

lib_dir=${RELATIVE_DIR}/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${lib_dir}:/usr/local/lib:/usr/local/lib64

echo "======================================================"

${RELATIVE_DIR}/bin/main

#if [ $# -eq 1 ]; then
#    echo "conf/$1.xml info.txt"
#    ${RELATIVE_DIR}/bin/main conf/$1.xml info.txt
#elif [ $# -eq 2 ]; then
#    echo "conf/$1.xml $2"
#    ${RELATIVE_DIR}/bin/main conf/$1.xml $2
#else
#    echo "conf/sample.xml info.txt"
#    ${RELATIVE_DIR}/bin/main conf/sample.xml info.txt
#fi

echo "======================================================"
