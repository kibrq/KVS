#pragma once

#include <sys/user.h>
#include <optional>

#include "KeyValue.hpp"
#include "log/Log.hpp"
#include "index/Index.hpp"
#include "storage/Storage.hpp"

template<size_t key_size, size_t value_size, size_t total_count>
class KeyValueStore {
private:
    constexpr static unsigned int log2_ceil(unsigned int value) {
        if (value <= 1) {
            return 1;
        }
        return 1 + log2_ceil(value / 2);
    }

    constexpr static std::size_t block_size = PAGE_SIZE;
    constexpr static std::size_t id_bits = log2_ceil(total_count);
    constexpr static std::size_t log_capacity = 0.02 * total_count;
    constexpr static std::size_t hash_size = std::max((std::size_t) 1, (std::size_t) (id_bits / 8));

private:
    using KeyValueL = KeyValue<key_size, value_size>;
    using KeyL = Key<key_size>;
    using ValueL = Value<value_size>;
    using TableBlockL = TableBlock<block_size, id_bits, key_size>;

public:
    explicit KeyValueStore();

    void add(const KeyValueL &);

    std::optional<ValueL> get(const KeyL &);

    void del(const KeyL &);

private:
    std::optional<unsigned int> get_id(const KeyL &);

    void check_log_size();

private:
    inline static RepositoryFactory get_factory_for_values() {
        std::string path(".dss/values");
        std::filesystem::create_directories(path);
        return RepositoryFactory(path);
    }

    inline static RepositoryFactory get_factory_for_table_blocks() {
        std::string path(".dss/index");
        std::filesystem::create_directories(path);
        return RepositoryFactory(path);
    }


private:
    Storage<ValueL, total_count> storage_m;
    Log<log_capacity, key_size, id_bits> log_m;
    Index<key_size, hash_size, block_size, id_bits> index_m;
};

#include "impl/KeyValueStoreImpl.hpp"
