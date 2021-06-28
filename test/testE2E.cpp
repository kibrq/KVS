#include "gtest/gtest.h"

#include <unordered_map>

#include "TestUtils.hpp"
#include "KeyValueStore.hpp"

TEST(E2ETest, Simple) {
    KeyValueStore<4, 4, 100000> store{};
    KeyValue<4, 4> record1 = createRecord<4, 4>("abcd", "aaaa");
    KeyValue<4, 4> record2 = createRecord<4, 4>("abcd", "bbbb");

    testOptionalHasNoValue(store.get(record1.getKey()));
    testOptionalHasNoValue(store.get(record2.getKey()));

    store.add(record1);
    testOptionalHasValue(store.get(record1.getKey()), record1.getValue());
    testOptionalHasValue(store.get(record2.getKey()), record1.getValue());

    store.add(record2);
    testOptionalHasValue(store.get(record1.getKey()), record2.getValue());
    testOptionalHasValue(store.get(record2.getKey()), record2.getValue());

    store.del(record1.getKey());
    testOptionalHasNoValue(store.get(record1.getKey()));
    testOptionalHasNoValue(store.get(record2.getKey()));

}

TEST(E2ETest, SmallLogCapacity) {
    KeyValueStore<4, 4, 100> store{};
    constexpr std::size_t records_count = 5;
    KeyValue<4, 4> records[records_count] = {
            createRecord<4, 4>("aaa1", "bbb1"),
            createRecord<4, 4>("aaa2", "bbb2"),
            createRecord<4, 4>("aaa3", "bbb3"),
            createRecord<4, 4>("aaa4", "bbb4"),
            createRecord<4, 4>("aaa5", "bbb5")
    };

    for (std::size_t i = 0; i < records_count; ++i) {
        store.add(records[i]);
    }

    for (std::size_t i = 0; i < records_count; ++i) {
        testOptionalHasValue(store.get(records[i].getKey()), records[i].getValue());
    }
}

TEST(E2ETest, SmallLogWithDel) {
    KeyValueStore<4, 4, 100> store{};
    constexpr std::size_t records_count = 5;

    KeyValue<4, 4> records[records_count] = {
            createRecord<4, 4>("aaa1", "bbb1"),
            createRecord<4, 4>("aaa2", "bbb2"),
            createRecord<4, 4>("aaa3", "bbb3"),
            createRecord<4, 4>("aaa4", "bbb4"),
            createRecord<4, 4>("aaa5", "bbb5")
    };

    for (std::size_t i = 0; i < records_count; ++i) {
        store.add(records[i]);
    }

    store.del(records[2].getKey());
    store.del(records[4].getKey());

    testOptionalHasNoValue(store.get(records[4].getKey()));
    testOptionalHasNoValue(store.get(records[2].getKey()));

    store.add(records[2]);
    store.add(records[4]);

    for (std::size_t i = 0; i < records_count; ++i) {
        testOptionalHasValue(store.get(records[i].getKey()), records[i].getValue());
    }
}

TEST(E2ETest, StressTest) {
    KeyValueStore<2, 16, 50> store{};
    std::unordered_map<std::string, std::string> records;

#if 1
    for (std::size_t i = 0; i < 21; ++i) {
        std::string key = createRandomString<2>();
        std::string value = createRandomString<16>();
        records[key] = value;
        store.add(createRecord<2, 16>(key, value));
    }
#endif

#if 1
    for (auto&[key, value] : records) {
        testOptionalHasValue(store.get(createKey<2>(key)), createValue<16>(value));
    }
#endif

#if 0
    for (std::size_t i = 0; i < 5000; ++i) {
        std::string key = createRandomString<2>();
        if (records.find(key) != records.end()) {
            testOptionalHasValue(store.get(createKey<2>(key)), createValue<16>(records[key]));
        } else {
            testOptionalHasNoValue(store.get(createKey<2>(key)));
        }
    }
#endif

#if 0

    for (std::size_t i = 0; i < 2000; ++i) {
        std::string key = createRandomString<2>();
        switch(rand() % 3) {
            case 0: {
                records[key] = createRandomString<16>();
                store.add(createRecord<2, 16>(key, records[key]));
                break;
            }
            case 1: {
                records.erase(key);
                store.del(createKey<2>(key));
                break;
            }
            default: {
                if (records.find(key) != records.end()) {
                    testOptionalHasValue(store.get(createKey<2>(key)), createValue<16>(records[key]));
                } else {
                    testOptionalHasNoValue(store.get(createKey<2>(key)));
                }
                break;
            }
        }
    }
#endif

}


