#pragma once

#include <cstring>
#include <memory>
#include <cstddef>

#include "KeyValue.hpp"
#include "LazyUnalignedArray.hpp"

template<size_t key_size>
class LazyUnalignedArray<Key<key_size>> {
public:
    constexpr static size_t bit_per_value = 8 * key_size;

    static Key<key_size> get(const char *src, size_t index) {
        std::unique_ptr<char[]> data = std::make_unique<char[]>(key_size);
        memcpy(data.get(), src + key_size * index, key_size);
        return Key<key_size>{std::move(data)};
    }

    static void set(char *dst, const Key<key_size> &value, size_t index) {
        memcpy(dst + key_size * index, value.getKey(), key_size);
    }
};
