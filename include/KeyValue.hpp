#pragma once

#include <cstddef>

template<size_t size>
class Key {
public:
    explicit Key(const char *key) : key{key} {}

    [[nodiscard]] const char *getKey() const {
        return key;
    }

    [[nodiscard]] consteval size_t getSize() const {
        return size;
    }

private:
    const char *key;
};

template<size_t size>
class Value {
public:
    explicit Value(const char *value) : value{value} {}

    [[nodiscard]] const char *getValue() const {
        return value;
    }

    [[nodiscard]] consteval size_t getSize() const {
        return size;
    }

private:
    const char *value;
};


template<size_t key_size, size_t value_size>
class KeyValue {
public:
    KeyValue(const char *key, const char *value) : key{key}, value{value} {}

    KeyValue(Key<key_size> key, Value<value_size> value) : key{key}, value{value} {}

    [[nodiscard]] Key<key_size> getKey() const {
        return key;
    }

    [[nodiscard]] Value<value_size> getValue() const {
        return value;
    }

private:
    Key<key_size> key;
    Value<value_size> value;
};
