#pragma once

#include "index/Filter.hpp"

template<std::size_t key_size>
class StubFilter : public virtual Filter<key_size> {
    virtual bool is_absent(const Key<key_size> &key) override {
        return false;
    }

    virtual void add(const Key<key_size> &key) override {

    };

    virtual void clear() override {

    }
};