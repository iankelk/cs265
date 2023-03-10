#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <cmath>
#include <functional>
#include <string>

#include "dynamic_bitset.hpp"
#include "data_types.hpp"

class BloomFilter {
public:
    BloomFilter(int capacity, double error_rate, int bitset_size);

    void add(const KEY_t key);
    bool contains(const KEY_t key);

private:
    int capacity;
    double error_rate;
    int bitset_size;
    int num_levels;
    int bits_per_level;
    DynamicBitset bits;
    std::hash<std::string> hasher;
};

#endif /* BLOOM_FILTER_HPP */
