#pragma once

#include <utility>

#include "serialization/SerializerSized.hpp"
#include "RepositoryFactory.hpp"
#include "TypedRepository.hpp"

template<SerializedSized T>
class TypedRepositoryFactory {
public:
    explicit TypedRepositoryFactory(RepositoryFactory &&factory) : factory_m{std::move(factory)} {}

    TypedRepository<T> get() {
        return TypedRepository<T>(factory_m.get());
    }

private:
    RepositoryFactory factory_m;
};