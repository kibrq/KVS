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
    constexpr static std::size_t key_size = 2;
    constexpr static std::size_t value_size = 16;
    constexpr static std::size_t N = 10000;
    KeyValueStore<key_size, value_size, N> store{};
    std::unordered_map<std::string, std::string> records;

#if 1
    for (std::size_t i = 0; i < 4500; ++i) {
        std::string key = createRandomString<key_size>();
        std::string value = createRandomString<value_size>();
        records[key] = value;
        store.add(createRecord<key_size, value_size>(key, value));
    }
#endif

#if 1
    // std::size_t i = 0;
    for (auto&[key, value] : records) {
        //std::cout << (i++) << " " << key << " " << value << '\n';
        testOptionalHasValue(store.get(createKey<key_size>(key)), createValue<value_size>(value));
    }
#endif

#if 1
    for (std::size_t i = 0; i < 10000; ++i) {
        std::string key = createRandomString<key_size>();
        if (records.contains(key)) {
            testOptionalHasValue(store.get(createKey<key_size>(key)), createValue<value_size>(records[key]));
        } else {
            testOptionalHasNoValue(store.get(createKey<key_size>(key)));
        }
    }
#endif

#if 1
    for (std::size_t i = 0; i < 5000; ++i) {
        std::string key = createRandomString<key_size>();
        switch ((rand() % 3)) {
            case 0: {
                store.del(createKey<key_size>(key));
                records.erase(key);
                break;
            }
            case 1: {
                std::string value = createRandomString<value_size>();
                store.add(createRecord<key_size, value_size>(key, value));
                records[key] = value;
                break;
            }
            case 2: {
                if (records.contains(key)) {
                    testOptionalHasValue(store.get(createKey<key_size>(key)), createValue<value_size>(records[key]));
                } else {
                    testOptionalHasNoValue(store.get(createKey<key_size>(key)));
                }
                break;
            }
        }
    }
#endif
}
