#pragma once

#include <cstring>

#include "KeyValue.hpp"
#include "Serializer.hpp"
#include "TableBlock.hpp"

template<size_t block_size, size_t id_bits, size_t key_size>
class Serializer<TableBlock<block_size, id_bits, key_size>> {
    using TBlock = TableBlock<block_size, id_bits, key_size>;
public:
    static constexpr size_t value_size = block_size;

    static TBlock read(char *data) {
        return TBlock{data};
    }

    static void write(TBlock block, char *dst) {
        memcpy(dst, block.data, block_size);
    }
};