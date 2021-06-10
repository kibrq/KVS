#include "gtest/gtest.h"
#include "Storage.hpp"
#include "TestUtils.hpp"
#include "ValueSerializer.hpp"

TEST(Storage, ValuesSimple) {
    constexpr size_t value_size = 4;
    constexpr size_t capacity = 4;
    Storage<Value<value_size>, capacity> storage(createTypedRepository<Value<value_size>>());
    auto val1 = createValue<value_size>("volk");
    auto val2 = createValue<value_size>("wolf");
    auto val3 = createValue<value_size>("1234");
    auto val4 = createValue<value_size>("'op'");
    auto val5 = createValue<value_size>("long");
    ASSERT_EQ(0, storage.write(val1));
    ASSERT_EQ(1, storage.write(val2));
    ASSERT_EQ(val1, storage.read(0));
    ASSERT_EQ(2, storage.write(val3));
    ASSERT_EQ(3, storage.write(val4));
    ASSERT_EQ(val1, storage.read(0));
    ASSERT_EQ(val2, storage.read(1));
    ASSERT_EQ(val3, storage.read(2));
    ASSERT_EQ(val4, storage.read(3));
    storage.update(2, val5);
    ASSERT_EQ(val5, storage.read(2));
    storage.remove(1);
    ASSERT_EQ(1, storage.write(val5));
    ASSERT_EQ(val5, storage.read(1));
}
