# Bareflank Hypervisor CPUID Count Example

## Description

This example demonstrates how to extend the Bareflank hypervisor to print
the number of CPUID instructions that have executed from when the hypervisor
was started, to when it was stopped. For more information on how Bareflank
extensions work, please see the following:

[API Documentation](http://bareflank.github.io/hypervisor/html/)

## Compilation / Usage

First, you'll need to set up and build the [core Bareflank Hypervisor](https://github.com/bareflank/hypervisor),
including [its SDK](https://github.com/bareflank/bfsdk).

Once you have a working Bareflank build environment, building the extension is 
easy:

```sh
git clone https://github.com/Bareflank/hypervisor_example_cpuidcount.git

cd hypervisor_example_cpuidcount
mkdir build
cd build

# Adjust the line below if you've installed to a non-default bfprefix location.
cmake .. -D DCMAKE_TOOLCHAIN_FILE=~/bfprefix/cmake/CMakeToolchain_VMM.txt
make install
```

We're now ready to start our extended hypervisor. As with Bareflank itself,
the easiest way to launch the hypervisor is with the ```bfm quick``` command:
but there's minor twist, as we'll need to instruct bfm to prefer our extended
libraries:

```sh
# From your build directory
sudo BF_LIBRARY_PATH="$(pwd)/lib" bfm quick
```

We can now use a vmcall to request the number of CPUIDs experienced thus far:

```sh
sudo bfm vmcall string json '{"get":"count"}'
```

The result should include the number of elapsed CPUID instructions:

```
received from vmm:
[
    154
]
```


