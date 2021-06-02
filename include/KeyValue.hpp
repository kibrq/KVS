#pragma once

class Key {
public:
    Key(const char *key, size_t size) : size{size}, key{key} {}

    [[nodiscard]] const char *getKey() const {
        return key;
    }

    [[nodiscard]] size_t getSize() const {
        return size
    }

    size_t size;
    const char *key;
};

class Value {
public:
    Value(const char *value, size_t size) : size{size}, value{value} {}

    [[nodiscard]] const char *getValue() const {
        return value;
    }

    [[nodiscard]] size_t getSize() const {
        return size;
    }

    size_t size;
    const char *value;
};

class KeyValue {
public:
    KeyValue(const char *key, size_t key_size, const char *value, size_t value_size)
            : key{key, key_size}, value{value, value_size} {}

    KeyValue(Key key, Value value) : key{key}, value{value} {}

    [[nodiscard]] Key getKey() const {
        return key;
    }

    [[nodiscard]] Value getValue() const {
        return value;
    }

    Key key;
    Value value;
};
