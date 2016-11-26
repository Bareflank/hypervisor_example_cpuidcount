//
// Bareflank Hypervisor Examples
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#ifndef EXIT_HANDLER_CPUIDCOUNT_H
#define EXIT_HANDLER_CPUIDCOUNT_H

#include <exit_handler/exit_handler_intel_x64.h>
#include <vmcs/vmcs_intel_x64_32bit_read_only_data_fields.h>

using namespace intel_x64;

class exit_handler_cpuidcount : public exit_handler_intel_x64
{
public:

    /// Default Constructor
    ///
    exit_handler_cpuidcount() :
        m_count(0)
    { }

    /// Destructor
    ///
    /// On destruction, this class prints out the total number of cpuids
    /// that have occurred after the VMM was started. Note that when the
    /// exit handler is destroyed, the vCPU's debug ring is already gone,
    /// so the "dump" command will not print out this message (both the
    /// constructors, and destructors have this issue). You will see this
    /// message being broadcast over serial.
    ///
    ~exit_handler_cpuidcount() override
    { bfdebug << "cpuid count = " << m_count << bfendl; }

    /// Handle CPUID
    ///
    /// When each cpuid is executed, we increment a count. This count will
    /// be shown when the exit handle is destroyed, or when it is
    /// queried via a vmcall. We also pass the the exit to the base class
    /// so that it can emulate cpuid, as well as other exits that might
    /// have occurred.
    ///
    void handle_exit(intel_x64::vmcs::value_type reason) override
    {
        if (reason == vmcs::exit_reason::basic_exit_reason::cpuid)
            m_count++;

        exit_handler_intel_x64::handle_exit(reason);
    }

    /// Handle JSON VMCall
    ///
    /// When the user executes a JSON based vmcall to the hypervisor,
    /// this function is called for us. This code demonstrates how to
    /// handle such requests. We check to see what kind of JSON command
    /// we got (which can be structured however you want), and return
    /// the cpuid count if the request is correct, otherwise I toss
    /// up an error.
    ///
    void handle_vmcall_data_string_json(
        vmcall_registers_t &regs, const json &str,
        const bfn::unique_map_ptr_x64<char> &omap) override
    {
        if (str["get"] == "count")
            return reply_with_json(regs, json{{"count", m_count}}, omap);

        throw std::runtime_error("unknown JSON command");
    }

private:

    int64_t m_count;
};

#endif
