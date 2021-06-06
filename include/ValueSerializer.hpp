#pragma once

#include <cstring>

#include "KeyValue.hpp"
#include "Serializer.hpp"

template<size_t size>
class Serializer<Value<size>> {
public:
    static constexpr size_t value_size = size;

    static Value<size> read(const char *data) {
        return Value<size>{data};
    }

    static void write(Value<size> value, char *dst) {
        memcpy(dst, value.getValue(), size);
    }
};