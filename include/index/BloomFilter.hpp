#pragma once

#include <cstddef>
#include <bitset>
#include <functional>
#include <cmath>

#include "util/XXHashWrapper.hpp"
#include "Filter.hpp"

template<std::size_t key_size, std::size_t elements_count, std::size_t bits_per_key>
class BloomFilter : public virtual Filter<key_size> {
private:
    using KeyL = Key<key_size>;

private:
    constexpr static std::size_t bitset_size = bits_per_key * elements_count;

    constexpr static double ln2 = 0.6931;
    constexpr static std::size_t hash_functions_count = static_cast<std::size_t>(bits_per_key * ln2) + 1;

    constexpr static uint64_t prime1 = 0x9E3779B185EBCA87ULL;
    constexpr static uint64_t prime2 = 0xC2B2AE3D27D4EB4FULL;

public:

    static long double expected_error() {

        // (1 - e^(- k * n / m))^k
        // k = hash_function_count
        // n = elements_count
        // m = bitset_size

        return std::pow(1 - std::exp(-(static_cast<long double>(hash_functions_count * elements_count) / bitset_size)),
                        hash_functions_count);
    }

private:
    std::bitset<bitset_size> data;

public:

    bool is_absent(const KeyL &key) override;

    void add(const KeyL &key) override;

    void clear() override;

private:
    struct BloomHash {
        XXHashWrapper<key_size> inner_hash;
        std::size_t val1, val2;

        explicit BloomHash(const KeyL &key);

        std::size_t operator()(std::size_t index);
    };
};

#include "BloomFilterImpl.hpp"