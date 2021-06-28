#pragma once

#include <algorithm>

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
Index<key_size, hash_size, block_size, id_bits>::EntryIterator::EntryIterator(Index &index, std::size_t block_index,
                                                                              std::size_t in_block) : index_m{index},
                                                                                                      block_index_m{
                                                                                                              block_index},
                                                                                                      in_block_m{
                                                                                                              in_block} {
    load_block();
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
void Index<key_size, hash_size, block_size, id_bits>::EntryIterator::load_block() {
    if (block_index_m >= index_m.sparse_table_m.size() - 1)
        return;

    current_block_m = index_m.repository_m.read(block_index_m).to_vector();

    auto compare_entries_by_hash = [this](const typename TBlock::Entry &e1, const typename TBlock::Entry &e2) {
        return index_m.compare_keys_by_hash(e1.key, e2.key) < 0;
    };
    std::sort(current_block_m.begin(), current_block_m.end(), compare_entries_by_hash);

}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::EntryIterator::operator++() -> EntryIterator & {
    ++in_block_m;
    if (in_block_m >= current_block_m.size()) {
        in_block_m = 0, ++block_index_m;
        load_block();
    }
    return *this;
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
bool Index<key_size, hash_size, block_size, id_bits>::EntryIterator::operator!=(const EntryIterator &it) {
    return in_block_m != it.in_block_m || block_index_m != it.block_index_m;
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::EntryIterator::operator*() -> typename TBlock::Entry && {
    typename TBlock::Entry *entry = current_block_m.data() + in_block_m;
    return std::move(*entry);
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::EntryIterator::operator->() -> typename TBlock::Entry * {
    return current_block_m.data() + in_block_m;
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::begin() -> EntryIterator {
    return EntryIterator(*this, 0, 0);
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::end() -> EntryIterator {
    return EntryIterator(*this, sparse_table_m.size() - 1, 0);
}
