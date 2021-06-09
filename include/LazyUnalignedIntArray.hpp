#pragma once

#include <cstddef>

#include "LazyUnalignedArray.hpp"
#include "LimitedUnsignedInt.hpp"

template<size_t bits>
class LazyUnalignedArray<LimitedUnsignedInt<bits>> {
public:
    constexpr static size_t bit_per_value = sizeof(unsigned int) * 8;

    static unsigned int get(const char *src, size_t index) {
        return reinterpret_cast<const unsigned int *>(src)[index];
    }

    static void set(char *dst, unsigned int value, size_t index) {
        reinterpret_cast<unsigned int *>(dst)[index] = value;
    }
};
