#include <vector>

#include "gtest/gtest.h"
#include "repositories/TypedRepository.hpp"
#include "KeyValue.hpp"
#include "index/TableBlock.hpp"
#include "TestUtils.hpp"

TEST(TypedRepository, ValuesSimple) {
    TypedRepository<Value<5>> repository = createTypedRepository<Value<5>>();
    Value<5> val1 = createValue<5>("text1");
    Value<5> val2 = createValue<5>("text2");
    Value<5> val3 = createValue<5>("omore");
    repository.write(0, val1);
    repository.write(1, val2);
    repository.write(2, val3);
    ASSERT_EQ(val1, repository.read(0));
    ASSERT_EQ(val2, repository.read(1));
    ASSERT_EQ(val3, repository.read(2));
    repository.write(1, val1);
    ASSERT_EQ(val1, repository.read(1));
    repository.write(1, val3);
    ASSERT_EQ(val3, repository.read(1));
    repository.close();
}

TEST(TypedRepository, TableBlockSimple) {
    constexpr size_t block_size = 4096;
    constexpr size_t id_bits = 9;
    constexpr size_t value_size = 4;
    TypedRepository<TableBlock<block_size, id_bits, value_size>> repository = createTypedRepository<TableBlock<block_size, id_bits, value_size>>();
    std::vector<TableBlock<block_size, id_bits, value_size>::Entry> values1;
    values1.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("tex1", 11));
    values1.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("tex2", 17));
    values1.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("more", 34));
    values1.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("te+1", 17));
    values1.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("bonk", 34));
    TableBlock<block_size, id_bits, value_size> block1(values1);
    std::vector<TableBlock<block_size, id_bits, value_size>::Entry> values2;
    values2.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("tex3", 7));
    values2.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("tex4", 80));
    values2.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("1111", 314));
    values2.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("....", 1));
    values2.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("rand", 342));
    TableBlock<block_size, id_bits, value_size> block2(values2);
    repository.write(0, block1);
    repository.write(1, block2);
    TableBlock<block_size, id_bits, value_size> block1copy = repository.read(0);
    for (const auto &entry : values1) {
        auto id = block1copy.get(entry.key);
        ASSERT_TRUE(id.has_value());
        ASSERT_EQ(entry.id, id.value());
    }
    TableBlock<block_size, id_bits, value_size> block2copy = repository.read(1);
    for (const auto &entry : values2) {
        auto id = block2copy.get(entry.key);
        ASSERT_TRUE(id.has_value());
        ASSERT_EQ(entry.id, id.value());
    }
    repository.close();
}


TEST(TypedRepository, TableBlockHard) {
    constexpr size_t block_size = 5;
    constexpr size_t id_bits = 8;
    constexpr size_t value_size = 2;

    RepositoryFactory factory(".");

    TypedRepository<TableBlock<block_size, id_bits, value_size>> repository1(factory.get());
    TypedRepository<TableBlock<block_size, id_bits, value_size>> repository2(factory.get());

    std::vector<TableBlock<block_size, id_bits, value_size>::Entry> values1;
    values1.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("aa", 0));
    repository1.write(0, TableBlock<block_size, id_bits, value_size>(values1));

    std::vector<TableBlock<block_size, id_bits, value_size>::Entry> values2;
    values2.emplace_back(createTableBlockEntry<block_size, id_bits, value_size>("ba", 1));
    repository1.write(1, TableBlock<block_size, id_bits, value_size>(values2));

    repository2.close();
    repository2 = std::move(repository1);

    TableBlock<block_size, id_bits, value_size> block1copy = repository2.read(0);
    for (const auto &entry : values1) {
        auto id = block1copy.get(entry.key);
        ASSERT_TRUE(id.has_value());
        ASSERT_EQ(entry.id, id.value());
    }

    TableBlock<block_size, id_bits, value_size> block2copy = repository2.read(1);
    for (const auto &entry : values2) {
        auto id = block2copy.get(entry.key);
        ASSERT_TRUE(id.has_value());
        ASSERT_EQ(entry.id, id.value());
    }

    repository2.close();
}

