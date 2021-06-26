#pragma once

#include <cstring>
#include <cstddef>
#include <memory>

template<size_t size>
class Key {
public:
    explicit Key(std::unique_ptr<char[]> key) : key{std::move(key)} {}

    [[nodiscard]] const char *getKey() const {
        return key.get();
    }

    [[nodiscard]] consteval size_t getSize() const {
        return size;
    }

    bool operator==(const Key<size> &other) const {
        return strncmp(key.get(), other.key.get(), size) == 0;
    }

private:
    std::unique_ptr<char[]> key;
};

template<size_t size>
class Value {
public:
    explicit Value(std::unique_ptr<char[]> value) : value{std::move(value)} {}

    [[nodiscard]] const char *getValue() const {
        return value.get();
    }

    [[nodiscard]] consteval size_t getSize() const {
        return size;
    }

    bool operator==(const Value<size> &other) const {
        return strncmp(value.get(), other.value.get(), size) == 0;
    }

private:
    std::unique_ptr<char[]> value;
};

#include "ValueSerializer.hpp"

template<size_t key_size, size_t value_size>
class KeyValue {
public:
    KeyValue(const char *key, const char *value) : key{key}, value{value} {}

    KeyValue(Key<key_size>&& key, Value<value_size>&& value) : key{std::move(key)}, value{std::move(value)} {}

    [[nodiscard]] const Key<key_size>& getKey() const {
        return key;
    }

    [[nodiscard]] const Value<value_size>& getValue() const {
        return value;
    }

private:
    Key<key_size> key;
    Value<value_size> value;
};

