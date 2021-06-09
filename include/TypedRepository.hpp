#pragma once

#include <cstddef>
#include <memory>

#include "SerializerSized.hpp"
#include "Repository.hpp"

template<SerializedSized T>
class TypedRepository {
private:
    static constexpr size_t size = Serializer<T>::value_size;
    Repository repository;
public:
    explicit TypedRepository(Repository &&repository) : repository{std::move(repository)} {}

    T read(size_t index) {
        std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
        repository.read(index * size, data.get(), size);
        return Serializer<T>::consume(std::move(data));
    }

    void write(size_t index, const T &value) {
        const char *serializedView = Serializer<T>::view(value);
        repository.write(index * size, serializedView, size);
    }

    void close() {
        repository.close();
    }
};
