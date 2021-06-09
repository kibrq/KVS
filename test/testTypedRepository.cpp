#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "RepositoryFactory.hpp"
#include "TypedRepository.hpp"
#include "KeyValue.hpp"
#include "ValueSerializer.hpp"
#include "TableBlock.hpp"
#include "TableBlockSerializer.hpp"

template<size_t size>
static Value<size> createValue(const std::string &value) {
    std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
    memcpy(data.get(), value.data(), size);
    return Value<size>(std::move(data));
}

TEST(TypedRepository, Values) {
    RepositoryFactory factory(".");
    TypedRepository<Value<5>> repository(factory.get());
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
}

template<size_t size>
static typename TableBlock<4096, 8, size>::Entry createTableBlockEntry(const std::string &value, unsigned int id) {
    std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
    memcpy(data.get(), value.data(), size);
    return {Key<size>(std::move(data)), id};
}


TEST(TypedRepository, TableBlock) {
    RepositoryFactory factory(".");
    TypedRepository<TableBlock<4096, 8, 4>> repository(factory.get());
    std::vector<TableBlock<4096, 8, 4>::Entry> values1;
    values1.emplace_back(createTableBlockEntry<4>("tex1", 11));
    values1.emplace_back(createTableBlockEntry<4>("tex2", 17));
    values1.emplace_back(createTableBlockEntry<4>("more", 34));
    values1.emplace_back(createTableBlockEntry<4>("te+1", 17));
    values1.emplace_back(createTableBlockEntry<4>("bonk", 34));
    TableBlock<4096, 8, 4> block1(values1);
    std::vector<TableBlock<4096, 8, 4>::Entry> values2;
    values2.emplace_back(createTableBlockEntry<4>("tex3", 7));
    values2.emplace_back(createTableBlockEntry<4>("tex4", 80));
    values2.emplace_back(createTableBlockEntry<4>("1111", 314));
    values2.emplace_back(createTableBlockEntry<4>("....", 1));
    values2.emplace_back(createTableBlockEntry<4>("rand", 342));
    TableBlock<4096, 8, 4> block2(values2);
    repository.write(0, block1);
    repository.write(1, block2);
    TableBlock<4096, 8, 4> block1copy = repository.read(0);
    for (const auto &entry : values1) {
        auto id = block1copy.get(entry.key);
        ASSERT_TRUE(id.has_value());
        ASSERT_EQ(entry.id, id.value());
    }
    TableBlock<4096, 8, 4> block2copy = repository.read(1);
    for (const auto &entry : values2) {
        auto id = block2copy.get(entry.key);
        ASSERT_TRUE(id.has_value());
        ASSERT_EQ(entry.id, id.value());
    }
}
