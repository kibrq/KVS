#include "gtest/gtest.h"
#include "log/FreePositionsManager.hpp"

TEST(FreePositionsManager, Simple) {
    constexpr size_t size = 128;
    FreePositionsManager<size> manager{};
    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(i, manager.get());
    }
    for (size_t i = 0; i < size; ++i) {
        manager.free(i);
    }
    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(i, manager.get());
    }
    std::vector<size_t> sorted{0, 1, 2, 8, 16, 17, 18, 23, 44};
    for (const auto &item : sorted) {
        manager.free(item);
    }
    for (const auto &item : sorted) {
        ASSERT_EQ(item, manager.get());
    }
    manager.free(20);
    ASSERT_EQ(20, manager.get());
    manager.free(10);
    manager.free(21);
    manager.free(30);
    ASSERT_EQ(21, manager.get());
}
