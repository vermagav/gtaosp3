#include "cache.h"
#include <stdlib.h> // for rand

// This function must be called before using the cache system
void Cache::init(common::CachePolicy p, int max_size) {
    if(p == common::NOT_SET || p == common::RANDOM 
        || p == common::FIFO || p == common::LRU) {
        this->_policy = p;
        this->_size_max = max_size;
        this->_size_remaining = max_size;
        this->_debug_num_replacements = 0;
    } else {
        printf("Error: Invalid cache policy selected!\n");
    }
}

// Debug function that flushes cache data, but does not alter init(). Used only for testing.
void Cache::flush_debug() {
    this->_cache_data.clear();
    this->_vector.clear();
    std::queue<std::string> empty;
    std::swap(this->_queue, empty);
    this->_deck.clear();
    this->_size_remaining = this->_size_max;
}

int Cache::add(std::string url, std::string data) {
    // Check if we have enough room to add new entry
    int size = data.size();
    
    if(size > this->_size_max) {
        printf("Warning: Could not add page to cache, size greater than total cache max.\n");
        return 0;
    }
    switch(this->_policy) {
        case common::NOT_SET:
        // Never delete entries, keep adding indefinitely over max limit.
        // Nothing to do here, as no special deletion rules apply.
        break;

        case common::RANDOM:
        // Delete entries at random till we have enough free space
        int random_element;
        while(size > this->_size_remaining) {
            // Select random element
            random_element = rand() % this->_vector.size();
            // Increment size remaining
            this->_size_remaining += this->_cache_data[ this->_vector[random_element] ].size();
            //printf("# TODO(remove, @add): Size remaining increased by %d due to deletion of cache entry\n", this->_cache_data[ this->_vector[random_element] ].size());
            // Remove from data structures
            this->_cache_data.erase(this->_vector[random_element]);
            this->_vector.erase(this->_vector.begin() + random_element);
            this->_debug_num_replacements++;
        }
        // We have enough space now
        this->_vector.push_back(url);
        break;

        case common::FIFO:
        // Delete oldest entries till we have enough free space
        while(size > this->_size_remaining) {
            // Increment size remaining
            this->_size_remaining += this->_cache_data[ this->_queue.front() ].size();
            //printf("# TODO(remove, @FIFO): Size remaining increased by %d due to deletion of cache entry\n", this->_cache_data[ this->_queue.front() ].size());
            // Remove from data structures
            this->_cache_data.erase(this->_queue.front());
            this->_queue.pop();
            this->_debug_num_replacements++;
        }
        // We have enough space now
        this->_queue.push(url);
        break;

        case common::LRU:
        // The back of the queue represents the least recently used (deletion)
        // The front of the queue is the most recently used (moved during addition)
        while(size > this->_size_remaining) {
            // Increment size remaining
            this->_size_remaining += this->_cache_data[ this->_deck.back() ].size();
            //printf("# TODO(remove, @LRU): Size remaining increased by %d due to deletion of cache entry\n", this->_cache_data[ this->_deck.back() ].size());
            // Remove from data structures
            this->_cache_data.erase(this->_deck.back());
            this->_deck.pop_back();
            this->_debug_num_replacements++;
        }
        // We have enough space now
        this->_deck.push_front(url);
        break;
        
        default:
        // No default case.
        break;
    }

    // If we reached this point, the appropriate cache policy was executed
    // Go ahead and update main cache data structure
    this->_size_remaining -= size;
    this->_cache_data[url] = data;
    //printf("# TODO(remove, @ALL): Size remaining DECREASED by %d to %d due to addition of cache entry\n", size, this->_size_remaining);

    // Sanity check: floor size remaining at -1 for NOT_SET cache policies
    if(this->_size_remaining < -1) {
        this->_size_remaining = -1;
    }

    // All done, return success
    return 1;
}

bool Cache::exists(std::string url) {
    // Returns 1 if found, 0 if not
    return this->_cache_data.count(url);
}

std::string Cache::fetch(std::string url) {
    if(this->_policy == common::LRU) {
        // Traverse back from the front (most recently used)
        // to the back (least recently used) of the deck
        for(auto i = this->_deck.begin(); i != this->_deck.end(); i++) {
            /*
             * This is only ever called when the cache policy is set to LRU
             * and Cache::exists() returned true. Thus, a backward traversal
             * from most recently used will hit most common/repeated cache hits early.
             * The worst case of O(n) is only realized when the least recently used 
             * cache entry is hit for a cache lookup and needs to be moved to the front
             * of the queue.
             */
            if(*i == url) {
                // Found the matching url in cache, move to front of deck
                this->_deck.erase(i);
                this->_deck.push_front(url);
                break;
            }
        }
    }

    return this->_cache_data[url];
}

void Cache::dump_stats() {
    // This stat isn't necessarily 1:1. A big incoming entry might warrant deleting 3 older cache entries to make enough space.
    printf("  Number of cache entries deleted for replacement: %d\n", this->_debug_num_replacements);
    printf("  Size remaining in cache: %d\n", this->_size_remaining);
}

