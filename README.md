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
cd ~/hypervisor
git clone https://github.com/Bareflank/hypervisor_example_cpuidcount.git

./tools/scripts/setup-<xxx>.sh --no-configure
sudo reboot

~/hypervisor/configure -m ~/hypervisor_example_cpuidcount/bin/cpuidcount.modules
make
```

To test out our extended version of Bareflank, all we need to do is run the
following make shortcuts:

```
make driver_load
make quick

ARGS="string json '{\"get\":\"count\"}'" make vmcall

make stop
make driver_unload
```
