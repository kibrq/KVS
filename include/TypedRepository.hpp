#pragma once

#include "SerializerSized.hpp"
#include "Repository.hpp"

template<SerializedSized T>
class TypedRepository {
private:
    static inline constexpr size_t size = Serializer<T>::value_size;
    Repository repository;
public:
    explicit TypedRepository(Repository &&repository) : repository{std::move(repository)} {}

    T read(size_t index) {
        char tmp[size];
        repository.read(index * size, tmp, size);
        return Serializer<T>::read(tmp);
    }

    void write(size_t index, T value) {
        char tmp[size];
        Serializer<T>::write(value, tmp);
        repository.write(index * size, tmp, size);
    }

    void close() {
        repository.close();
    }
};
