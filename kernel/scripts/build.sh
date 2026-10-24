#!/bin/bash

# This script is intended to be launched from the project root.
# Unexpected results may happen otherwise.

set -euo pipefail

OPT_HELP="--help"
OPT_BUILD="--build"
OPT_CLEAN="--clean"
OPT_REBUILD="--rebuild"

case "$1" in
    "$OPT_HELP")
        echo "Kernel build manager."
        echo "$OPT_HELP            display this message"
        echo "$OPT_BUILD           compile and link source code"
        echo "$OPT_CLEAN           remove build files"
        echo "$OPT_REBUILD         clean and build"
        ;;
    "$OPT_BUILD")
        make TOOLCHAIN="$(pwd)/make/toolchain.mk"
        ;;
    "$OPT_CLEAN")
        make clean
        ;;
    "$OPT_REBUILD")
        make clean && make TOOLCHAIN="$(pwd)/make/toolchain.mk"
        ;;
    *)
        echo "Invalid option. Use --help for more info."
        ;;
esac