#include "rpc_service.h"
#include "common.h"
#include <vector>
#include <iostream>
#include <fstream>

// A client-side function that runs all of our test scenarios. Called from: rpc_service_client.cpp
// We pipe the output of dump_stats() on server side to a text file to analyze the results.
void batch_test(rpc::rpc_serviceClient* rpc_handle) {
    // Variables to store our test values
    std::vector<common::CachePolicy> cache_policies;
    std::vector<common::CacheSize> cache_sizes;
    std::vector<std::string> batch_files;

    // Populate test variables
    cache_policies.push_back(common::NOT_SET);
    cache_policies.push_back(common::RANDOM);
    cache_policies.push_back(common::FIFO);
    cache_policies.push_back(common::LRU);

    cache_sizes.push_back(common::HALF_MB);
    cache_sizes.push_back(common::ONE_MB);
    cache_sizes.push_back(common::THREE_MB);

    batch_files.push_back("batch1.txt");
    batch_files.push_back("batch2.txt");

    // Other variables
    std::string return_value;

    /* The following nest of 4 for() loops is okay as n = 4, 3 and 2
     * for the top 3 loops so overall complexity isn't bad.
     */

    // For each cache policy...
    for(auto& policy : cache_policies) {
        // For each cache size
        for(auto& size : cache_sizes) {
            // For each batch file
            for(auto& file : batch_files) {
                // Restart the proxy server, flush previous cache to get fresh stats
                rpc_handle->restart_debug(policy, size);
                
                // Process each line in the batch file (1 url per line)
                std::ifstream infile;
                infile.open(file);
                for(std::string line; getline(infile, line);/**/) {
                    return_value = "";
                    // Query the proxy server
                    rpc_handle->fetch(return_value, line);
                }
                
                // Cleanup
                infile.close();

                // Dump stats to console on server side (this is a debug function)
                rpc_handle->dump_stats(file);
            }
        }
    }
}
