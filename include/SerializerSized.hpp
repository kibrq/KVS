#pragma once

#include <concepts>
#include <limits>
#include "Serializer.hpp"

template<typename Serializer, typename T>
concept SerializerSized =
std::same_as<decltype(Serializer::value_size), const std::size_t>
&& requires(T object, const char *src, char *dst) {
    { Serializer::read(src) } -> std::same_as<T>;
    { Serializer::write(object, dst) };
};

template<typename T>
concept SerializedSized = SerializerSized<Serializer<T>, T>;
