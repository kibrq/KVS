#pragma once

#include <cstddef>

#include "FreePositionsManager.hpp"
#include "TypedRepository.hpp"

template<typename T, size_t size>
class Storage {
public:
    explicit Storage(TypedRepository<T> repository) : repository{std::move(repository)} {}

    T read(size_t index) {
        return repository.read(index);
    }

    size_t write(const T &value) {
        size_t index = free_positions.get();
        repository.write(index, value);
        return index;
    }

    void update(size_t index, const T &value) {
        repository.write(index, value);
    }

    void remove(size_t index) {
        free_positions.free(index);
    }

private:
    FreePositionsManager<size> free_positions{};
    TypedRepository<T> repository;
};