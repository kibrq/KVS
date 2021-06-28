#pragma once

#include <limits>

#include "KeyValue.hpp"

template<size_t size>
class KeyAction {
public:
    static constexpr size_t reserved_id = std::numeric_limits<size_t>::max();

    explicit KeyAction(Key<size> key) : key_m{std::move(key)}, id_m{reserved_id} {}

    KeyAction(Key<size> key, size_t id) : key_m{std::move(key)}, id_m{id} {}

    [[nodiscard]] const Key<size> & view_key() const {
        return key_m;
    }

    [[nodiscard]] Key<size> consume_key() {
        return std::move(key_m);
    }

    [[nodiscard]] bool is_removed() const {
        return id_m == reserved_id;
    }

    [[nodiscard]] size_t id() const {
        return id_m;
    }

private:
    Key<size> key_m;
    size_t id_m;
};