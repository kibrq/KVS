#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "TypedRepository.hpp"
#include "RepositoryFactory.hpp"
#include "KeyValue.hpp"
#include "TableBlock.hpp"
#include "SerializerSized.hpp"

template<size_t size>
Value<size> createValue(const std::string &value);

template<size_t size>
Key<size> createKey(const std::string &value);

template<SerializedSized T>
TypedRepository<T> createTypedRepository();

template<size_t block_size, size_t id_bits, size_t size>
typename TableBlock<block_size, id_bits, size>::Entry createTableBlockEntry(const std::string &value, unsigned int id);

#include "TestUtilsImpl.hpp"
