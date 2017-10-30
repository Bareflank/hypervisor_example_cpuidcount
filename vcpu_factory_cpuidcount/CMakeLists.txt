# ------------------------------------------------------------------------------
# CMake Includes
# ------------------------------------------------------------------------------

include(${CMAKE_INSTALL_PREFIX}/cmake/CMakeGlobal_Includes.txt)

# ------------------------------------------------------------------------------
# Custom Flags
# ------------------------------------------------------------------------------

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

add_executable(example_vmm vcpu_factory_cpuidcount.cpp)

if(BUILD_SHARED_LIBS)
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
endif()

if(BUILD_STATIC_LIBS)
    target_link_libraries(example_vmm -Wl,--whole-archive bfvmm_main -Wl,--no-whole-archive)
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
endif()

# ------------------------------------------------------------------------------
# Install
# ------------------------------------------------------------------------------

install(TARGETS example_vmm DESTINATION ${INSTALL_BIN_DIR})

# ------------------------------------------------------------------------------
# Output Dir
# ------------------------------------------------------------------------------

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
