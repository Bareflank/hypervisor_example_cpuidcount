#
# Bareflank Hypervisor
# Copyright (C) 2015 Assured Information Security, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

cmake_minimum_required(VERSION 3.6)
project(eapis C CXX)

include(${CMAKE_INSTALL_PREFIX}/cmake/CMakeGlobal_Project.txt)
include(${CMAKE_INSTALL_PREFIX}/cmake/CMakeGlobal_Includes.txt)

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

add_executable(example_vmm vcpu_factory_cpuidcount.cpp)

target_link_libraries(example_vmm -Wl,--whole-archive bfvmm_main -Wl,--no-whole-archive)
target_link_libraries(example_vmm bfvmm_vcpu_factory)
target_link_libraries(example_vmm bfvmm_vcpu)
target_link_libraries(example_vmm bfvmm_vcpu_manager)
target_link_libraries(example_vmm bfvmm_exit_handler)
target_link_libraries(example_vmm bfvmm_vmcs)
target_link_libraries(example_vmm bfvmm_vmxon)
target_link_libraries(example_vmm bfvmm_memory_manager)
target_link_libraries(example_vmm bfvmm_support)
target_link_libraries(example_vmm bfvmm_serial)
target_link_libraries(example_vmm bfvmm_debug_ring)
target_link_libraries(example_vmm bfvmm_intrinsics)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")
