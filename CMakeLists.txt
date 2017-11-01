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

include(ExternalProject)

if(NOT CYGWIN)
    set(SUDO sudo)
endif()

# ------------------------------------------------------------------------------
# Bareflank Hypervisor
# ------------------------------------------------------------------------------

ExternalProject_Add(
    hypervisor
    GIT_REPOSITORY      https://github.com/Bareflank/hypervisor.git
    GIT_TAG             dev
    GIT_SHALLOW         1
    PREFIX              ${CMAKE_BINARY_DIR}/external/hypervisor/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/external/hypervisor/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/external/hypervisor/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/hypervisor/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/hypervisor/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/external/hypervisor/build
    INSTALL_COMMAND     ""
    UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
)

# ------------------------------------------------------------------------------
# Extension
# ------------------------------------------------------------------------------

# TODO
#
# The following args will be removed once the new build system changes are
# in as the prefix should know:
# - what the source directory is for the hypervisor that built the prefix
# - what the build directory is for the hypervisor that built the prefix
# - what the toolchain was

list(APPEND EXTENSION_ARGS
    -DBAREFLANK_SOURCE_DIR=${CMAKE_BINARY_DIR}/external/hypervisor/src
    -DBAREFLANK_BINARY_DIR=${CMAKE_BINARY_DIR}/external/hypervisor/build
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/external/hypervisor/src/bfsdk/cmake/CMakeToolchain_VMM_40.txt
)

ExternalProject_Add(
    src
    CMAKE_ARGS          ${EXTENSION_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/src/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/src/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/src/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/src/download
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/src/build
    UPDATE_COMMAND      ${CMAKE_COMMAND} -E echo "checking for updates"
    INSTALL_COMMAND     ""
    DEPENDS             hypervisor
)

# ------------------------------------------------------------------------------
# BFM Shortcuts
# ------------------------------------------------------------------------------

if(NOT WIN32)
    add_custom_target(stop COMMAND ${SUDO} ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix/bin/bfm stop)
    add_custom_target(unload COMMAND ${SUDO} ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix/bin/bfm unload)
    add_custom_target(dump COMMAND ${SUDO} ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix/bin/bfm dump)
    add_custom_target(status COMMAND ${SUDO} ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix/bin/bfm status)

    add_custom_target(quick)
    add_custom_command(TARGET quick COMMAND ${SUDO} ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix/bin/bfm load src/build/example_vmm)
    add_custom_command(TARGET quick COMMAND ${SUDO} ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfprefix/bin/bfm start)
endif()

# ------------------------------------------------------------------------------
# Driver Shortcuts
# ------------------------------------------------------------------------------

if(NOT WIN32)
    add_custom_target(driver_build COMMAND make driver_build > /dev/null VERBATIM WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfdriver/build)
    add_custom_target(driver_load COMMAND make driver_load > /dev/null VERBATIM WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfdriver/build)
    add_custom_target(driver_unload COMMAND make driver_unload > /dev/null VERBATIM WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfdriver/build)
    add_custom_target(driver_clean COMMAND make driver_clean > /dev/null VERBATIM WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/hypervisor/build/bfdriver/build)

    add_custom_target(driver_quick)
    add_custom_command(TARGET driver_quick COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target driver_unload)
    add_custom_command(TARGET driver_quick COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target driver_clean)
    add_custom_command(TARGET driver_quick COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target driver_build)
    add_custom_command(TARGET driver_quick COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target driver_load)
endif()
