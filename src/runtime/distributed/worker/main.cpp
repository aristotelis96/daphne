/*
 *  Copyright 2021 The DAPHNE Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include <iostream>

#include "WorkerImpl.h"
#include "WorkerImplGRPCAsync.h"
#include "WorkerImplGRPCSync.h"


int main(int argc, char *argv[])
{
    DaphneUserConfig user_config{};
    auto logger = std::make_unique<DaphneLogger>(user_config);

    std::string dist_backend = ""; // Variable to store the value of dist_backend

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--dist_backend=") == 0) {
            dist_backend = arg.substr(15); // Extract the value after the "=" sign
        }
    }

    if (argc < 2 || dist_backend.empty()) {
        std::cout << "Usage: " << argv[0] << " --dist_backend=<str> <Address:Port>" << std::endl;
        exit(1);
    }

    auto addr = argv[argc - 1]; // Get the last argument as the address

    // TODO choose specific implementation based on arguments or config file
    WorkerImpl *service;
    if (dist_backend == "sync-gRPC"){
        service = new WorkerImplGRPCSync(addr, user_config);
        std::cout << "Started Distributed Worker with synchronous gRPC on `" << addr << "`\n";
    } else if (dist_backend == "async-gRPC") {
        service = new WorkerImplGRPCAsync(addr, user_config);
        std::cout << "Started Distributed Worker with asynchronous gRPC on `" << addr << "`\n";
    } else {
        std::cout << "We currently support only \"sync-gRPC\" or \"async-gRPC\"" << std::endl;
    }
    
    service->Wait();

    return 0;
}