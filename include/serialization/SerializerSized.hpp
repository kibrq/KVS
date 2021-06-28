#pragma once

#include <cstddef>
#include <concepts>
#include <memory>

#include "Serializer.hpp"

template<typename Serializer, typename T>
concept SerializerSized =
std::same_as<decltype(Serializer::value_size), const std::size_t>
&& requires(T object, std::unique_ptr<char[]> src, char *dst) {
    { Serializer::consume(std::move(src)) } -> std::same_as<T>;
    { Serializer::view(object) } -> std::same_as<const char *>;
};

template<typename T>
concept SerializedSized = SerializerSized<Serializer<T>, T>;
