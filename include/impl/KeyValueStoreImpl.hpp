#pragma once

#include "index/BloomFilter.hpp"
#include "log/Log.hpp"
#include "index/Index.hpp"

template<std::size_t key_size, std::size_t value_size, std::size_t total_count>
KeyValueStore<key_size, value_size, total_count>::KeyValueStore() : storage_m(
        TypedRepository<ValueL>(get_factory_for_values().get())), log_m{},
                                                                    index_m(std::make_unique<BloomFilter<key_size, total_count, 5>>(),
                                                                            std::make_unique<TypedRepositoryFactory<TableBlockL>>(
                                                                                    get_factory_for_table_blocks())) {}


template<std::size_t key_size, std::size_t value_size, std::size_t total_count>
std::optional<unsigned int> KeyValueStore<key_size, value_size, total_count>::get_id(const KeyL &key) {
    auto log_result = log_m.get(key);
    if (log_result.is_removed())
        return std::nullopt;
    if (log_result.is_found())
        return log_result.get();
    return index_m.get(key);
}

template<std::size_t key_size, std::size_t value_size, std::size_t total_count>
void KeyValueStore<key_size, value_size, total_count>::check_log_size() {
    if (log_m.size() >= log_capacity) {
        index_m.apply(log_m.summarize());
        log_m = Log<log_capacity, key_size, id_bits>();
    }
}


template<std::size_t key_size, std::size_t value_size, std::size_t total_count>
void KeyValueStore<key_size, value_size, total_count>::add(const KeyValueL &key_value) {
    std::optional<std::size_t> result = get_id(key_value.getKey());
    if (result.has_value()) {
        storage_m.update(result.value(), key_value.getValue());
    } else {
        log_m.add(key_value.getKey(), storage_m.write(key_value.getValue()));
    }

    check_log_size();
}

template<std::size_t key_size, std::size_t value_size, std::size_t total_count>
void KeyValueStore<key_size, value_size, total_count>::del(const KeyL &key) {
    std::optional<std::size_t> id = get_id(key);
    if (id.has_value()) {
        log_m.remove(key);
        storage_m.remove(id.value());
    }
    check_log_size();
}

template<std::size_t key_size, std::size_t value_size, std::size_t total_count>
auto KeyValueStore<key_size, value_size, total_count>::get(const KeyL &key) -> std::optional<ValueL> {
    std::optional<std::size_t> id = get_id(key);
    if (!id.has_value())
        return std::nullopt;
    return storage_m.read(id.value());
}
