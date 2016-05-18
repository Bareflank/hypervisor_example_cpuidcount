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

#include <exit_handler/exit_handler_intel_x64.h>

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
    /// that have occured after the VMM was started. Note that when the
    /// exit handler is destroyed, the vCPU's debug ring is already gone,
    /// so the "dump" command will not print out this message (both the
    /// constructors, and destructors have this issue). You will see this
    /// message being broadcast over serial.
    ///
    virtual ~exit_handler_cpuidcount()
    {
        bfdebug << "cpuid count = " << m_count << bfendl;
    }

    /// Handle CPUID
    ///
    /// When each cpuid is executed, we increment a count. This cound will
    /// be shown when the exit handle is destroyed. Note that we call the
    /// base class here to handle cpuid for us. We could however run the
    /// cpuid instruction ourselves, and for example, change the result if
    /// we wanted.
    ///
    virtual void 
    handle_cpuid() override
    {
        m_count++;
        exit_handler_intel_x64::handle_cpuid();
    }

private:

    int64_t m_count;
};
