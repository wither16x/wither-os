# to build using this file run:
# make TOOLCHAIN="$(pwd)/make/toolchain.mk"

NASM := nasm
CC := clang
CXX := clang++
LD := clang++

NASMFLAGS :=
CCFLAGS :=
CXXFLAGS :=
LDFLAGS := -fuse-ld=lld