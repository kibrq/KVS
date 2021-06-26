#include <Log.hpp>
#include "gtest/gtest.h"

#include "StubFilter.hpp"
#include "TestUtils.hpp"
#include "index/Index.hpp"


template<typename T>
void check_optional_present(const std::optional<T> &opt, const T &val) {
    ASSERT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), val);
}

template<typename T>
void check_optional_not_present(const std::optional<T> &opt) {
    EXPECT_FALSE(opt.has_value());
}

template<std::size_t key_size>
using FilterL = StubFilter<key_size>;

template<std::size_t block_size, std::size_t id_bits, std::size_t key_size>
using FactoryL = TypedRepositoryFactory<TableBlock<block_size, id_bits, key_size>>;

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
Index<key_size, hash_size, block_size, id_bits> get_index() {
    return Index<key_size, hash_size, block_size, id_bits>(std::make_unique<FilterL<key_size>>(),
                                                           std::make_unique<FactoryL<block_size, id_bits, key_size>>(
                                                                   RepositoryFactory(".")));
}


TEST(TestIndex, EMPTY) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t hash_size = 3;
    constexpr std::size_t block_size = 4096;
    constexpr std::size_t id_bits = 8;

    auto index = get_index<key_size, hash_size, block_size, id_bits>();

    EXPECT_EQ(std::nullopt, index.get(createKey<key_size>("AAAA")));
    EXPECT_EQ(std::nullopt, index.get(createKey<key_size>("BBBB")));
}

TEST(TestIndex, APPLY_WITH_EMPTY) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t hash_size = 3;
    constexpr std::size_t block_size = 4096;
    constexpr std::size_t id_bits = 8;

    auto index = get_index<key_size, hash_size, block_size, id_bits>();

    Log<10, key_size, id_bits> log{};
    Key<4> key1 = createKey<key_size>("abcd");
    Key<4> key2 = createKey<key_size>("doge");
    Key<4> key3 = createKey<key_size>("coin");
    Key<4> key4 = createKey<key_size>(")!?(");
    log.add(key1, 1);
    log.add(key2, 2);
    log.remove(key3);
    log.add(key4, 1);

    index.apply(log.summarize());

    check_optional_present<unsigned int>(index.get(key1), 1);
    check_optional_present<unsigned int>(index.get(key2), 2);
    check_optional_not_present<unsigned int>(index.get(key3));
    check_optional_present<unsigned int>(index.get(key4), 1);
    check_optional_not_present<unsigned int>(index.get(createKey<key_size>("AAAA")));
    check_optional_not_present<unsigned int>(index.get(createKey<key_size>("BBBB")));
}

TEST(TestIndex, APPLY_WITH_NON_EMPTY) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t hash_size = 3;
    constexpr std::size_t block_size = 4096;
    constexpr std::size_t id_bits = 8;

    auto index = get_index<key_size, hash_size, block_size, id_bits>();

    Log<10, key_size, id_bits> log{};
    Key<4> key1 = createKey<key_size>("abcd");
    Key<4> key2 = createKey<key_size>("doge");
    Key<4> key3 = createKey<key_size>("coin");
    Key<4> key4 = createKey<key_size>(")!?(");
    log.add(key1, 1);
    log.add(key2, 2);
    log.add(key3, 3);
    log.add(key4, 4);

    index.apply(log.summarize());

    check_optional_present<unsigned int>(index.get(key1), 1);
    check_optional_present<unsigned int>(index.get(key2), 2);
    check_optional_present<unsigned int>(index.get(key3), 3);
    check_optional_present<unsigned int>(index.get(key4), 4);

    log = Log<10, key_size, id_bits>();
    log.add(key3, 10);
    log.remove(key1);
    log.add(key4, 11);

    index.apply(log.summarize());

    check_optional_not_present<unsigned int>(index.get(key1));
    check_optional_present<unsigned int>(index.get(key3), 10);
    check_optional_present<unsigned int>(index.get(key4), 11);
    check_optional_present<unsigned int>(index.get(key2), 2);
}

TEST(TestIndex, APPLY_WITH_LITTLE_BLOCK_SIZE) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t hash_size = 3;
    constexpr std::size_t block_size = 4096;
    constexpr std::size_t id_bits = 8;

    auto index = get_index<key_size, hash_size, block_size, id_bits>();

    Log<200, key_size, id_bits> log{};
    std::vector<Key<key_size>> keys;
    for (int i = 0; i < 100; ++i) {
        keys.push_back(createKey<key_size>(std::to_string(i) + "aaa"));
        log.add(keys.back(), i);
    }

    index.apply(log.summarize());

    for (int i = 0; i < 100; ++i) {
        check_optional_present<unsigned int>(index.get(keys[i]), i);
    }
    check_optional_not_present<unsigned int>(index.get(createKey<key_size>("abcd")));

}
