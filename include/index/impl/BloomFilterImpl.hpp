#pragma once


template<std::size_t key_size, std::size_t elements_count, std::size_t bits_per_key>
BloomFilter<key_size, elements_count, bits_per_key>::BloomHash::BloomHash(const KeyL &key) {
    val1 = inner_hash(key, prime1);
    val2 = inner_hash(key, prime2);
}

template<std::size_t key_size, std::size_t elements_count, std::size_t bits_per_key>
std::size_t BloomFilter<key_size, elements_count, bits_per_key>::BloomHash::operator()(std::size_t index) {
    ++index;
    return (val1 ^ (val2 << index) * index) % bitset_size;
}

template<std::size_t key_size, std::size_t elements_count, std::size_t bits_per_key>
bool BloomFilter<key_size, elements_count, bits_per_key>::is_absent(const KeyL &key) {
    BloomHash hash(key);
    bool res = false;
    for (std::size_t i = 0; i < hash_functions_count; ++i) {
        res |= !data[hash(i)];
    }

    return res;
}

template<std::size_t key_size, std::size_t elements_count, std::size_t bits_per_key>
void BloomFilter<key_size, elements_count, bits_per_key>::add(const KeyL &key) {
    BloomHash hash(key);
    for (std::size_t i = 0; i < hash_functions_count; ++i) {
        data.set(hash(i));
    }
}

template<std::size_t key_size, std::size_t elements_count, std::size_t bits_per_key>
void BloomFilter<key_size, elements_count, bits_per_key>::clear() {
    data.reset();
}
