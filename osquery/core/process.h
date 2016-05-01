/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#pragma once

#include <string>
#include <boost/optional.hpp>

#include <osquery/core.h>

namespace osquery {

#ifdef WIN32
using PlatformPidType = HANDLE;
#else
using PlatformPidType = pid_t;
#endif

const PlatformPidType kInvalidPid = (PlatformPidType) -1;

class PlatformProcess {
  public:
    PlatformProcess(PlatformPidType id);
    PlatformProcess(PlatformProcess&& src);
    ~PlatformProcess();

    PlatformPidType nativeHandle() { return id_; }
    
    // TODO(#1991): Consider making kill() return an enumeration for more granularity if an
    //              error happens to occur.
    // TODO(#1991): Also, consider adding an argument for exit code so that clients can specificy
    //               the process exit code for the terminating process.
    bool kill();
    
    bool isValid() { return (id_ != kInvalidPid); }

    bool operator==(const PlatformProcess& process);
    bool operator!=(const PlatformProcess& process);
    
    static PlatformProcess launchWorker(const std::string& exec_path, const std::string& name);
    static PlatformProcess launchExtension(const std::string& exec_path, 
                                           const std::string& extension,
                                           const std::string& extensions_socket,
                                           const std::string& extensions_timeout,
                                           const std::string& extensions_interval,
                                           const std::string& verbose);
    static PlatformProcess fromPlatformPid(PlatformPidType id);
    
  private:
    PlatformPidType id_ = kInvalidPid;
};

PlatformProcess getCurrentProcess();
PlatformProcess getLauncherProcess();

void processSleep(unsigned int msec);

bool isLauncherProcessDead(PlatformProcess& launcher);
bool setEnvVar(const std::string& name, const std::string& value);
bool unsetEnvVar(const std::string& name);
boost::optional<std::string> getEnvVar(const std::string& name);

// TODO(#1991): Missing register signal handlers function
// TODO(#1991): Missing waitpid functionality
}
