#include "gtest/gtest.h"
#include "unalignedArray/UnalignedIntArray.hpp"

template<size_t size>
using IntArray = UnalignedArray<LimitedUnsignedInt<size>>;

TEST(LazyUnalignedIntArray, Simple) {
    char data[3];
    unsigned int values[5]{1, 2, 15, 0, 6};
    for (size_t i = 0; i < 5; ++i) {
        IntArray<4>::set(data, values[i], i);
    }
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(values[i], IntArray<4>::get(data, i));
    }
}

TEST(LazyUnalignedIntArray, Simple17) {
    char data[17];
    unsigned int values[8]{1, 2, 15, 0, 6, (1u << 17) - 1, 32456, 99999};
    for (size_t i = 0; i < 8; ++i) {
        IntArray<17>::set(data, values[i], i);
    }
    for (size_t i = 0; i < 8; ++i) {
        ASSERT_EQ(values[i], IntArray<17>::get(data, i));
    }
}
