#ifndef __COMMON_H
#define __COMMON_H

// Common variables used by both server and client
namespace common {

    typedef int CachePolicy;
    typedef int CacheSize;

    const CachePolicy NOT_SET = 0;          // No cache policy, cache everything indefinitely
    const CachePolicy RANDOM = 1;           // Randomly delete older entries
    const CachePolicy FIFO = 2;             // Delete oldest entries first
    const CachePolicy LRU = 3;              // Delete least recent entries first

    const CacheSize HALF_MB = 524288;       // 1024 * 512
    const CacheSize ONE_MB = 1048576;       // 1024 * 1024
    const CacheSize TWO_MB = 2097152;       // 1024 * 1024 * 2
    const CacheSize THREE_MB = 3145728;     // 1024 * 1024 * 3

}

#endif // __COMMON_H
