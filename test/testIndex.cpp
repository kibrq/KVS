#include <Log.hpp>
#include "gtest/gtest.h"

#include "StubFilter.hpp"
#include "TestUtils.hpp"
#include "index/Index.hpp"
#include "TableBlockSerializer.hpp"


template<typename T>
void check_optional_present(const std::optional<T> &opt, const T &val) {
    ASSERT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), val);
}

template<typename T>
void check_optional_not_present(const std::optional<T> &opt) {
    EXPECT_FALSE(opt.has_value());
}


TEST(TestIndex, EMPTY) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t hash_size = 3;
    constexpr std::size_t block_size = 4096;
    constexpr std::size_t id_bits = 8;

    StubFilter<key_size> filter;
    TypedRepositoryFactory<TableBlock<block_size, id_bits, key_size>> factory(RepositoryFactory("."));

    Index<key_size, hash_size, block_size, id_bits> index(filter, factory);

    EXPECT_EQ(std::nullopt, index.get(createKey<key_size>("AAAA")));
    EXPECT_EQ(std::nullopt, index.get(createKey<key_size>("BBBB")));
}

TEST(TestIndex, APPLY_WITH_EMPTY) {
    constexpr std::size_t key_size = 4;
    constexpr std::size_t hash_size = 3;
    constexpr std::size_t block_size = 4096;
    constexpr std::size_t id_bits = 8;

    StubFilter<key_size> filter;
    TypedRepositoryFactory<TableBlock<block_size, id_bits, key_size>> factory(RepositoryFactory("."));

    Index<key_size, hash_size, block_size, id_bits> index(filter, factory);

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

    StubFilter<key_size> filter;
    TypedRepositoryFactory<TableBlock<block_size, id_bits, key_size>> factory(RepositoryFactory("."));

    Index<key_size, hash_size, block_size, id_bits> index(filter, factory);

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
