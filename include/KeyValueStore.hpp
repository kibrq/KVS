#pragma once

#include <optional>

#include "KeyValue.hpp"

template<size_t key_size, size_t value_size>
class KeyValueStore {
public:
    using KeyValueL = KeyValue<key_size, value_size>;
    using KeyL = Key<key_size>;

    void add(const KeyValueL &);

    std::optional<KeyValueL> get(const Key &);

    void del(const KeyL &);
};
