# to build using this file run:
# make TOOLCHAIN="$(pwd)/make/toolchain.mk"

NASM := nasm
CC := clang
LD := clang

NASMFLAGS :=
CCFLAGS :=
LDFLAGS := -fuse-ld=lld