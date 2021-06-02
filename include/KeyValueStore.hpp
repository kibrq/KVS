#pragma once

#include <optional>

class KeyValueStore {
    void add(const KeyValue &);
    std::optional<KeyValue> get(const Key &);
    void del(const Key&);
};