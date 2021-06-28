#pragma once

#include "TestUtils.hpp"

#include <algorithm>

template<size_t size>
Value<size> createValue(const std::string &value) {
    std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
    memcpy(data.get(), value.data(), size);
    return Value<size>(std::move(data));
}

template<size_t size>
Key<size> createKey(const std::string &value) {
    std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
    memcpy(data.get(), value.data(), size);
    return Key<size>(std::move(data));
}

template <size_t key_size, size_t value_size>
KeyValue<key_size, value_size> createRecord(const std::string &key, const std::string &value) {
    return KeyValue<key_size, value_size>(createKey<key_size>(key), createValue<value_size>(value));
}


template<std::size_t size>
std::string createRandomString() {
    auto rand_char = []() {
        return 'a' + (rand() % 26);
    };
    std::string str(size, 0);
    std::generate_n(str.begin(), size, rand_char);
    return str;
}

template<SerializedSized T>
TypedRepository<T> createTypedRepository() {
    return TypedRepository<T>(RepositoryFactory(".").get());
}

template<size_t block_size, size_t id_bits, size_t size>
typename TableBlock<block_size, id_bits, size>::Entry createTableBlockEntry(const std::string &value, unsigned int id) {
    return typename TableBlock<block_size, id_bits, size>::Entry{createKey<size>(value), id};
}

template <typename T>
void testOptionalHasValue(const std::optional<T>& opt, const T&value) {
    EXPECT_TRUE(opt.has_value());
    if (opt.has_value()) {
        EXPECT_TRUE(opt.value() == value);
    }
}

template <typename T>
void testOptionalHasNoValue(const std::optional<T>& opt) {
    EXPECT_FALSE(opt.has_value());
}
