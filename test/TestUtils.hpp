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

template <size_t key_size, size_t value_size>
KeyValue<key_size, value_size> createRecord(const std::string &key, const std::string &value);

template <std::size_t size>
std::string createRandomString();

template<SerializedSized T>
TypedRepository<T> createTypedRepository();

template<size_t block_size, size_t id_bits, size_t size>
typename TableBlock<block_size, id_bits, size>::Entry createTableBlockEntry(const std::string &value, unsigned int id);

template <typename T>
void testOptionalHasValue(const std::optional<T>& opt, const T&value);

template <typename T>
void testOptionalHasNoValue(const std::optional<T>& opt);

#include "TestUtilsImpl.hpp"
