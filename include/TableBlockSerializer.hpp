#pragma once

#include "Serializer.hpp"
#include "TableBlock.hpp"

template<size_t block_size, size_t id_bits, size_t key_size>
class Serializer<TableBlock<block_size, id_bits, key_size>> {
    using TBlock = TableBlock<block_size, id_bits, key_size>;
public:
    static constexpr size_t value_size = block_size;

    static TBlock consume(std::unique_ptr<char[]> data) {
        return TBlock{std::move(data)};
    }

    static const char* view(const TBlock &block) {
        return block.data.get();
    }
};