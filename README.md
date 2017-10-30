# Bareflank Hypervisor CPUID Count Example

## Description

This example demonstrates how to extend the Bareflank hypervisor to print
the number of CPUID instructions that have executed from when the hypervisor
was started, to when it was stopped. For more information on how Bareflank
extensions work, please see the following:

[API Documentation](http://bareflank.github.io/hypervisor/html/)

## Compilation / Usage

To setup our extension, run the following (assuming Linux):

```
cd ~/
git clone https://github.com/Bareflank/hypervisor.git
mkdir ~/hypervisor/build; cd ~/hypervisor/build
cmake ..; make; make driver_quick

cd ~/
git clone https://github.com/Bareflank/hypervisor_example_cpuidcount.git
mkdir ~/hypervisor_example_cpuidcount/build; cd ~/hypervisor_example_cpuidcount/build

cmake \
    -DBAREFLANK_SOURCE_DIR=~/hypervisor \
    -DBAREFLANK_BINARY_DIR=~/hypervisor/build \
    -DCMAKE_INSTALL_PREFIX=~/hypervisor/build/bfprefix/ \
    -DCMAKE_TOOLCHAIN_FILE=~/hypervisor/bfsdk/cmake/CMakeToolchain_VMM_40.txt \
    ..

make

sudo ~/hypervisor/build/bfprefix/bin/bfm load example_vmm
```

To test out our extended version of Bareflank, all we need to do is run the
following (assuming Linux):

```
sudo ~/hypervisor/build/bfprefix/bin/bfm load example_vmm
sudo ~/hypervisor/build/bfprefix/bin/bfm start

sudo ~/hypervisor/build/bfprefix/bin/bfm status
sudo ~/hypervisor/build/bfprefix/bin/bfm dump

sudo ~/hypervisor/build/bfprefix/bin/bfm stop
sudo ~/hypervisor/build/bfprefix/bin/bfm dump
```
