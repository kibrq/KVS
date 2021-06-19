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

template<std::size_t size>
std::string createRandomKey() {
    auto rand_char = []() {
        return rand() % 255;
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
