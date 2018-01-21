//
// Bareflank Hypervisor Examples
// Copyright (C) 2015 Assured Information Security, Inc.
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

#include <vcpu/vcpu_factory.h>

#include <vcpu/arch/intel_x64/vcpu.h>
#include <hve/arch/intel_x64/exit_handler/exit_handler.h>

class exit_handler_cpuidcount : public exit_handler_intel_x64
{
public:

    /// Default Constructor
    ///
    exit_handler_cpuidcount() = default;

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
    {
        bfdebug_ndec(0, "cpuid count", m_count);
    }

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
        if (reason == intel_x64::vmcs::exit_reason::basic_exit_reason::cpuid) {
            m_count++;
        }

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
    void
    handle_vmcall_data_string_json(
        const json &ijson, json &ojson) override
    {
        if (ijson.at("get") == "count") {
            ojson = {m_count};
            return;
        }

        throw std::runtime_error("unknown JSON command");
    }

private:

    uint64_t m_count{0};
};

/// Custom VCPU.
///
/// This tells Bareflank to use our exit handler instead of the one
/// that Bareflank provides by default.
///
std::unique_ptr<vcpu>
vcpu_factory::make_vcpu(vcpuid::type vcpuid, user_data *data)
{
    bfignored(data);

    return std::make_unique<vcpu_intel_x64>(
               vcpuid,
               nullptr,
               nullptr,
               std::make_unique<exit_handler_cpuidcount>(),
               nullptr,
               nullptr);
}
