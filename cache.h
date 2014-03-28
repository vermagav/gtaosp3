#ifndef __CACHE_H
#define __CACHE_H

#include "common.h"
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <deque>

class Cache {
  private:
    // Our main hashmap data structure with O(1) lookup
    // Maps url -> page_data
    std::unordered_map<std::string, std::string> _cache_data;
    
    // A queue of URL strings to determine position in cache queue
    // Used for FIFO elimination
    std::queue<std::string> _queue;

    // A vector of URL strings to fetch a random key from
    // Used for Random elimination
    std::vector<std::string> _vector;

    // A deque of URL strings to order by least recently used
    // Used for LRU elimination
    std::deque<std::string> _deck;

    // Current policy
    common::CachePolicy _policy;

    // Variables for controlling cache size (in bytes / num of chars)
    int _size_max;
    int _size_remaining;

    // Testing data
    int _debug_num_replacements;

  public:
    void init(common::CachePolicy p, int max_size);
    void flush_debug();
    bool ready();
    int add(std::string url, std::string data);
    bool exists(std::string url);
    std::string fetch(std::string url);
    void dump_stats();
};

#endif // __CACHE_H
