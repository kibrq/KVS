#pragma once

#include "KeyValue.hpp"

template<std::size_t key_size>
class Filter {
public:
    virtual ~Filter() = default;

    virtual bool is_absent(const Key<key_size> &key) = 0;

    virtual void add(const Key<key_size> &key) = 0;

    virtual void clear() = 0;

};
