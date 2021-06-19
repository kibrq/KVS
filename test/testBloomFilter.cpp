
#include <gtest/gtest.h>

#include <unordered_set>
#include <cstdlib>

#include "index/BloomFilter.hpp"
#include "TestUtils.hpp"


TEST(TestBloomFilter, Basics) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t elements_count = 1000;
    constexpr std::size_t bits_per_key = 5;
    using KeyL = Key<key_size>;

    BloomFilter<key_size, elements_count, bits_per_key> filter;

    KeyL key1 = createKey<key_size>("abcd");
    KeyL key2 = createKey<key_size>("efgh");
    KeyL key3 = createKey<key_size>("qwer");

    filter.add(key1), filter.add(key2), filter.add(key3);

    EXPECT_FALSE(filter.is_absent(key1));
    EXPECT_FALSE(filter.is_absent(key2));
    EXPECT_FALSE(filter.is_absent(key3));
}


TEST(TestBloomFilter, CollisionRate) {
    constexpr std::size_t key_size = 8;
    constexpr std::size_t elements_count = 1000;
    constexpr std::size_t bits_per_key = 5;
    constexpr std::size_t iterations = 1e5;
    constexpr long double eps = 0.0095;

    for (std::size_t _ = 0; _ < 10; ++_) {
        BloomFilter<key_size, elements_count, bits_per_key> filter;
        std::unordered_set<std::string> set;

        for (std::size_t i = 0; i < elements_count; ++i) {
            std::string s = createRandomKey<key_size>();
            set.insert(s), filter.add(createKey<key_size>(s));
        }

        std::size_t mistakes = 0;
        for (std::size_t i = 0; i < iterations; ++i) {
            std::string s = createRandomKey<key_size>();
            mistakes += !set.contains(s) && !filter.is_absent(createKey<key_size>(s));
        }

        long double actual_error = static_cast<long double>(mistakes) / iterations;
        long double expected_error = BloomFilter<key_size, elements_count, bits_per_key>::expected_error();

        EXPECT_LE(actual_error, expected_error + eps);
    }
}

