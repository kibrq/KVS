#pragma once

#include <cstring>
#include <memory>

#include "serialization/Serializer.hpp"

template<size_t size>
class Serializer<Value<size>> {
public:
    static constexpr size_t value_size = size;

    static Value<size> consume(std::unique_ptr<char[]> data) {
        return Value<size>{std::move(data)};
    }

    static const char *view(const Value<size> &value) {
        return value.getValue();
    }
};
