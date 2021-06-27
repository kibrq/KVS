#pragma once

template<std::size_t key_size>
std::size_t XXHashWrapper<key_size>::operator()(const KeyL &key, uint64_t seed) {
    return XXH64(key.getKey(), key_size, seed);
}
