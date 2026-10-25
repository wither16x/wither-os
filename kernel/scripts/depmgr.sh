#!/bin/bash

set -euo pipefail

DEPS_DIR="dependencies"

OPT_HELP="--help"
OPT_DOWNLOAD="--download"
OPT_REMOVE="--remove"
OPT_INSTALL="--install"
OPT_UNINSTALL="--uninstall"
OPT_GET="--get"

download_dependency() {
    if [[ ! -d "$DEPS_DIR/$3" ]]; then
        git clone "$1" --depth=1 --branch="$2" "$DEPS_DIR/$3"
    fi
}

download_dependencies() {
    download_dependency https://github.com/limine-bootloader/limine-protocol trunk \
        limine-protocol
    download_dependency https://github.com/osdev0/freestanding-c-hdrs trunk \
        freestanding-c-hdrs
    download_dependency https://github.com/osdev0/freestanding-cxx-hdrs trunk \
        freestanding-cxx-hdrs
    download_dependency https://github.com/Limine-Bootloader/Limine v11.x-binary \
        limine
}

install_dependencies() {
    if [[ -d "$DEPS_DIR/limine-protocol" ]]; then
        cp -v -r "$DEPS_DIR/limine-protocol/include/" "include/limine/"
    fi

    if [[ -d "$DEPS_DIR/freestanding-c-hdrs" ]]; then
        cp -v -r "$DEPS_DIR/freestanding-c-hdrs/include/" "include/standard/"
    fi

    if [[ -d "$DEPS_DIR/freestanding-cxx-hdrs" ]]; then
        cp -v -r "$DEPS_DIR/freestanding-cxx-hdrs/include/" "include/standard/cxx/"
    fi

    if [[ -d "$DEPS_DIR/limine" ]]; then
        make -C "$DEPS_DIR/limine"
    fi
}

mkdir -p "$DEPS_DIR"

case "$1" in
    "$OPT_HELP")
        echo "Kernel dependency manager."
        echo "$OPT_HELP            display this message"
        echo "$OPT_DOWNLOAD        download missing dependencies"
        echo "$OPT_REMOVE          remove downloaded dependencies"
        echo "$OPT_INSTALL         install downloaded dependencies"
        echo "$OPT_UNINSTALL       uninstall dependencies"
        echo "$OPT_GET             download and install missing dependencies"
        ;;
    "$OPT_DOWNLOAD")
        download_dependencies
        ;;
    "$OPT_REMOVE")
        rm -rf "$DEPS_DIR"
        ;;
    "$OPT_INSTALL")
        install_dependencies
        ;;
    "$OPT_UNINSTALL")
        if [[ -d "include/limine" ]]; then
            rm -rf include/limine
        fi

        if [[ -d "include/standard" ]]; then
            rm -rf include/standard
        fi
        ;;
    "$OPT_GET")
        download_dependencies
        install_dependencies
        ;;
    *)
        echo "Invalid option. Use --help for more info."
        ;;
esac