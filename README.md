# WitherOS
An independent operating system written from scratch.
## Build instructions
### Dependency management
The build system uses makefiles and shell scripts. Before building anything, you must obtain the required dependencies. Operating system components, such as the kernel, have a `scripts` directory in which is located a `depmgr.sh` file. It is a command line tool to easily download, install or remove the component's dependencies.  
All `depmgr.sh` scripts should provide the same interface. Run them with the `--help` option to get a list of available options.  
For example, the `--get` option downloads and installs all required dependencies.  
Note that you must run these scripts from the component's root directory. Otherwise the dependencies would be located in the wrong place, which would lead to unexpected results.
### Toolchain, source code compilation and link
Once you installed the dependencies, you can now proceed to the source code compilation. It is recommended to use the `Makefile` at the root of the repository, as it can build every components and supports easy configuration.  
In order to compile and link the code, you need a toolchain. It is defined in `make/toolchain.mk` as makefile macros. You can change their values if you want, but note that the default values are the ones I use to compile the project, so they should work perfectly.
Once you have a toolchain, you can run
```sh
make TOOLCHAIN="$(pwd)/make/toolchain.mk" # change path if needed
```
If something went wrong during the build, it is recommended to restart it completely. You can do this very easily by running
```sh
make clean
```
### Image generation
To make the OS bootable, you need to make an image. Currently, only ISO images are supported by the build system.  
The base `Makefile` has an `iso` target which generates an ISO image in `build/images/` if the required source code has been compiled before.
### For developers
If you are working on WitherOS and using VSCodium or VSCode, you can use `.vscode/tasks.json`, which contains predefined tasks for cleaning, building and running the OS. You can make them even easier to run by installing the Tasks extension.
## Run instructions
WitherOS has not been tested on real hardware. However you can make a bootable USB stick very easily using a program like Balena Etcher.  
Otherwise, you can run the OS in an emulator. The base `Makefile` has a target for running it in `qemu` with the `OVMF` firmware.