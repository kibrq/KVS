#pragma once

#include <cstddef>
#include <cassert>
#include <optional>

#include "KeyValue.hpp"
#include "LimitedUnsignedInt.hpp"
#include "LazyUnalignedIntArray.hpp"
#include "LazyUnalignedKeyArray.hpp"

template<size_t block_size, size_t id_bits, size_t key_size>
class TableBlock {
    using IdType = LimitedUnsignedInt<id_bits>;
    using Id = typename IdType::PlainType;
    using IdArray = LazyUnalignedArray<IdType>;
    using KeyArray = LazyUnalignedArray<Key<key_size>>;
    static_assert(KeyArray::bit_per_value % 8 == 0);
public:
    static constexpr Id reserved_id = IdType::max_value;
    static constexpr size_t max_size = (block_size * 8) / (IdArray::bit_per_value + KeyArray::bit_per_value);

    struct Entry {
        Key<key_size> key;
        Id id;
    };

    explicit TableBlock(std::vector<Entry> entries) : size{entries.size()} {
        assert(size <= max_size);
        for (size_t i = 0; i < size; ++i) {
            KeyArray::set(keyArray(), entries[i].key, i);
            IdArray::set(idArray(), entries[i].id, i);
        }
        if (size < max_size) {
            IdArray::set(idArray(), reserved_id, entries.size());
        }
    }

    std::optional<Id> get(Key<key_size> key) {
        for (size_t i = 0; i < size; ++i) {
            if (KeyArray::get(keyArray(), i) == key) {
                return IdArray::get(idArray(), i);
            }
        }
        return {};
    }

    std::vector<Entry> to_vector() {
        std::vector<Entry> vec;
        vec.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            vec.emplace_back(KeyArray::get(keyArray(), i), IdArray::get(idArray(), i));
        }
        return vec;
    }

    friend class Serializer<TableBlock<block_size, id_bits, key_size>>;

private:
    explicit TableBlock(std::unique_ptr<char[]> data) : size{0}, data{std::move(data)} {
        while (size <= max_size && IdArray::get(idArray(), size) != reserved_id) ++size;
        --size;
    }

    static constexpr size_t idArrayOffset = max_size * KeyArray::bit_per_value / 8;

    [[nodiscard]] inline char *idArray() const {
        return data.get() + idArrayOffset;
    }

    [[nodiscard]] inline char *keyArray() const {
        return data.get();
    }

    size_t size;
    std::unique_ptr<char[]> data = std::make_unique<char[]>(block_size);
};