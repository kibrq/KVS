#pragma once

#include <cstddef>

#include "LazyUnalignedArray.hpp"
#include "LimitedUnsignedInt.hpp"

template<size_t bits>
class LazyUnalignedArray<LimitedUnsignedInt<bits>> {
public:
    constexpr static size_t bit_per_value = bits;

    static unsigned int get(const char *src, size_t index) {
        size_t pos = bits * (index + 1) - 1;
        unsigned int sum = 0;
        for (size_t i = 0; i < bits; ++i) {
            if (get_bit(src, pos - i)) {
                sum += 1u << i;
            }
        }
        return sum;
    }

    static void set(char *dst, unsigned int value, size_t index) {
        size_t pos = bits * (index + 1) - 1;
        for (size_t i = 0; i < bits; ++i) {
            set_bit(dst, pos - i, value & (1u << i));
        }
    }

private:
    static bool get_bit(const char *src, size_t bit) {
        return (static_cast<unsigned int>(src[bit / 8]) & (1u << (7 - bit % 8)));
    }

    static void set_bit(char *dst, size_t bit, bool value) {
        if (value) {
            dst[bit / 8] = static_cast<char>(static_cast<unsigned int>(dst[bit / 8]) | (1u << (7 - bit % 8)));
        } else {
            dst[bit / 8] = static_cast<char>(static_cast<unsigned int>(dst[bit / 8]) & (~(1u << (7 - bit % 8))));
        }
    }
};
