#pragma once

#include "LazyUnalignedArray.hpp"

template<size_t key_size>
class LazyUnalignedArray<Key<key_size>> {
public:
    constexpr static size_t bit_per_value = 8 * key_size;

    static Key<key_size> get(const char *src, size_t index) {
        return Key<key_size>{src + key_size * index};
    }

    static void set(char *dst, Key<key_size> value, size_t index) {
        memcpy(dst + key_size * index, value.getKey(), key_size);
    }
};
