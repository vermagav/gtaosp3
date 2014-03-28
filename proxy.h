#ifndef __PROXY_H
#define __PROXY_H

#include "cache.h"
#include <curl/curl.h>

class Proxy {
  private:
    // Our main cache handle
    Cache _cache;
    
    // Curl variables
    CURL* _curl_handle;
    CURLcode _curl_response;

    // Testing data
    int _debug_total_requests;
    int _debug_cache_hits;
    double _debug_total_time;
  public:
    void init(common::CachePolicy policy, common::CacheSize cache_size);
    void restart_debug(common::CachePolicy policy, common::CacheSize cache_size);
    std::string fetch(std::string url);
    void dump_stats(std::string message);
};

#endif // __PROXY_H
