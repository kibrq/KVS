#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>

#include "KeyValue.hpp"
#include "Filter.hpp"
#include "TypedRepositoryFactory.hpp"
#include "TypedRepository.hpp"
#include "LimitedUnsignedInt.hpp"
#include "TableBlock.hpp"
#include "KeyAction.hpp"

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
class Index {
private:
    using Hash = std::string;
    using TBlock = TableBlock<block_size, id_bits, key_size>;
    using IdType = LimitedUnsignedInt<id_bits>;
    using Id = typename IdType::PlainType;

private:
    std::unique_ptr<Filter<key_size>> filter_m;
    std::unique_ptr<TypedRepositoryFactory<TBlock>> factory_m;

    TypedRepository<TBlock> repository_m;
    std::vector<Hash> sparse_table_m;

public:
    // Assumes that repository is empty
    Index(std::unique_ptr<Filter<key_size>>&&, std::unique_ptr<TypedRepositoryFactory<TBlock>> &&);

    std::optional<Id> get(const Key<key_size> &key);

    void apply(std::vector<KeyAction<key_size>> &&actions);

private:
    Hash calculate_hash(const Key<key_size> &key);

    bool compare_keys_by_hash(const Key<key_size> &, const Key<key_size> &);

    struct EntryIterator : std::iterator<std::input_iterator_tag, typename TBlock::Entry> {
        explicit EntryIterator(Index &index, std::size_t block_index, std::size_t in_block);

        EntryIterator &operator++();

        bool operator!=(const EntryIterator &);

        // Can be called once for each element

        typename TBlock::Entry &&operator*();

        typename TBlock::Entry *operator->();

    private:
        void load_block();

        Index &index_m;
        std::size_t block_index_m, in_block_m;
        std::vector<typename TBlock::Entry> current_block_m;
    };

    EntryIterator begin();

    EntryIterator end();

};

#include "EntryIteratorImpl.hpp"
#include "IndexImpl.hpp"
