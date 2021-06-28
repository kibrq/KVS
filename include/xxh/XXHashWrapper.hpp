#pragma once

#define XXH_INLINE_ALL
#include "xxhash.h"
#undef XXH_INLINE_ALL

#include <cstddef>

#include "KeyValue.hpp"

template<std::size_t key_size>
struct XXHashWrapper {
private:
    using KeyL = Key<key_size>;
public:

    std::size_t operator()(const KeyL &, uint64_t seed);

};

#include "XXHashWrapperImpl.hpp"