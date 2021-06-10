#pragma once

#include <cassert>
#include <vector>
#include <memory>

#include "KeyValue.hpp"
#include "LazyUnalignedIntArray.hpp"
#include "LazyUnalignedKeyArray.hpp"
#include "KeyAction.hpp"
#include "SearchResult.hpp"

template<size_t capacity, size_t key_size, size_t id_bits>
class Log {
    using IdType = LimitedUnsignedInt<id_bits>;
    using Id = typename IdType::PlainType;
    using IdArray = LazyUnalignedArray<IdType>;
    using KeyArray = LazyUnalignedArray<Key<key_size>>;
public:
    static constexpr Id reserved_id = IdType::max_value;

    void add(const Key<key_size> &key, Id id) {
        assert(id != reserved_id);
        append(key, id);
    }

    void remove(const Key<key_size> &key) {
        append(key, reserved_id);
    }

    SearchResult<Id> get(const Key<key_size> &key) {
        for (size_t i = 0; i < size_m; ++i) {
            if (KeyArray::get(key_array.get(), i) == key) {
                Id id = IdArray::get(id_array.get(), i);
                if (id == reserved_id) {
                    return SearchResult<Id>::removed();
                } else {
                    return SearchResult<Id>::found(id);
                }
            }
        }
        return SearchResult<Id>::notFound();
    }

    size_t size() {
        return size_m;
    }

    std::vector<KeyAction<key_size>> summarize() {
        std::vector<KeyAction<key_size>> summary;
        summary.reserve(size_m);
        for (size_t i = 0; i < size_m; ++i) {
            Key<key_size> key = KeyArray::get(key_array.get(), i);
            Id id = IdArray::get(id_array.get(), i);
            if (id == reserved_id) {
                summary.emplace_back(std::move(key));
            } else {
                summary.emplace_back(std::move(key), id);
            }
        }
        return summary;
    }

private:
    void append(const Key<key_size> &key, Id id) {
        KeyArray::set(key_array.get(), key, size_m);
        IdArray::set(id_array.get(), id, size_m);
        ++size_m;
    }

    size_t size_m = 0;
    std::unique_ptr<char[]> id_array = std::make_unique<char[]>((capacity * IdArray::bit_per_value + 7) / 8);
    std::unique_ptr<char[]> key_array = std::make_unique<char[]>((capacity * KeyArray::bit_per_value + 7) / 8);
};