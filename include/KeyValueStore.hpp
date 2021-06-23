#pragma once

#include <sys/user.h>
#include <optional>

#include "KeyValue.hpp"
#include "Log.hpp"
#include "index/Index.hpp"

template<size_t key_size, size_t value_size, size_t total_count>
class KeyValueStore {
private:
    constexpr static unsigned int div_ceil(unsigned int a, unsigned int b) {
        return (a + b - 1) / b;
    }

    constexpr static unsigned int log2_ceil(unsigned int value) {
        if (value <= 1) {
            return 1;
        }
        return 1 + log2_ceil(value / 2);
    }

    constexpr static std::size_t block_size = PAGE_SIZE;
    constexpr static std::size_t key_value_per_block = div_ceil(block_size, key_size + value_size);
    constexpr static std::size_t log_capacity = 0.02 * total_count;
    constexpr static std::size_t id_bits = log2_ceil(total_count);
    constexpr static std::size_t hash_size = div_ceil(log2_ceil(total_count / key_value_per_block), 8);

public:
    using KeyValueL = KeyValue<key_size, value_size>;
    using KeyL = Key<key_size>;

    explicit KeyValueStore();

    void add(const KeyValueL &);

    std::optional<KeyValueL> get(const KeyL &);

    void del(const KeyL &);

private:
    Log<log_capacity, key_size, id_bits> log;
    Index<key_size, hash_size, block_size, id_bits> index;
};

#include "KeyValueStoreImpl.hpp"
