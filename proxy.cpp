#include "proxy.h"
#include <ctime>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void Proxy::init(common::CachePolicy policy, int cache_size) {    
    // Initialize cache
    this->_cache.init(policy, cache_size);

    // Reset test variables
    this->_debug_total_requests = 0;
    this->_debug_cache_hits = 0;
    this->_debug_total_time = 0.0;

    // Debug message
    printf("\n\n> Proxy server initialized with cache policy %d and cache size %d\n\n", policy, cache_size);
}

// Debug function that restarts the proxy service. Used only for testing.
void Proxy::restart_debug(common::CachePolicy policy, common::CacheSize cache_size) {
    // Flush all data in cache and reinitialize with new values
    this->_cache.flush_debug();
    this->init(policy, cache_size);
}

std::string Proxy::fetch(std::string url) {
    this->_debug_total_requests++;
    clock_t start_time = clock();
    std::string data;
    int result;
    // -- Start fetch process

    // Check if url already exists in cache
    if(this->_cache.exists(url)) {
        // Found, return http data from cache
        this->_debug_cache_hits++;
        return this->_cache.fetch(url);
    } else {
        // Not found, fetch http data
        this->_curl_handle = curl_easy_init();
        if(this->_curl_handle) {
            curl_easy_setopt(this->_curl_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(this->_curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(this->_curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(this->_curl_handle, CURLOPT_WRITEDATA, &data);
            this->_curl_response = curl_easy_perform(this->_curl_handle);
            curl_easy_cleanup(this->_curl_handle);
        }

        // Add to cache
        result = this->_cache.add(url, data);

        // Return http data
    }

    // -- End fetch process
    clock_t end_time = clock();
    double time_elapsed = double(end_time - start_time) / CLOCKS_PER_SEC;
    this->_debug_total_time += time_elapsed;
    
    if(result == 1) {
        return data;
    } else {
        printf("Error adding entry to cache!\n");
    }
}

void Proxy::dump_stats(std::string message) {
    printf("\n\n ----------------------------------------------- \n\n");
    printf("  %s\n", message.c_str());
    printf("  Total number of requests: %d\n", this->_debug_total_requests);
    printf("  Number of cache hits: %d\n", this->_debug_cache_hits);
    printf("  Number of fresh lookups: %d\n", (this->_debug_total_requests - this->_debug_cache_hits));
    printf("  Average elapsed time per request: %f\n", (this->_debug_total_time / _debug_total_requests));
    this->_cache.dump_stats();
    printf("\n\n ----------------------------------------------- \n\n");
}
