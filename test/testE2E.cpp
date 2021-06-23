#include "gtest/gtest.h"

#include "TestUtils.hpp"

#include "KeyValueStore.hpp"

TEST(E2ETest, Simple) {
    KeyValueStore<4, 2048, 100000> store();
    store.add(createKey<4>("abcd"));
}