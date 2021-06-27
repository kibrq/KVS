cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER g++-10)

project(KVS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wconversion")

include_directories(include/ lib/)

enable_testing()

add_subdirectory(test)
add_subdirectory(benchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(KVS
        src/Main.cpp
        src/repositories/RepositoryFactory.cpp
        src/repositories/Repository.cpp
        include/KeyValue.hpp
        include/KeyValueStore.hpp
        include/repositories/RepositoryFactory.hpp
        include/repositories/Repository.hpp
        include/serialization/Serializer.hpp
        include/serialization/SerializerSized.hpp
        include/repositories/TypedRepository.hpp
        include/serialization/ValueSerializer.hpp
        include/unalignedArray/UnalignedArray.hpp
        include/unalignedArray/UnalignedKeyArray.hpp
        include/unalignedArray/UnalignedIntArray.hpp
        include/util/LimitedUnsignedInt.hpp
        include/index/TableBlock.hpp
        include/serialization/TableBlockSerializer.hpp
        include/storage/Storage.hpp
        include/log/FreePositionsManager.hpp
        include/log/Log.hpp
        include/util/KeyAction.hpp
        include/util/SearchResult.hpp
        include/index/Index.hpp
        include/index/Filter.hpp
        include/index/impl/IndexImpl.hpp
        include/repositories/TypedRepositoryFactory.hpp
        include/index/impl/EntryIteratorImpl.hpp
        include/index/BloomFilter.hpp
        include/index/impl/BloomFilterImpl.hpp
        lib/xxhash.h
        include/xxh/XXHashWrapper.hpp
        include/xxh/XXHashWrapperImpl.hpp
        include/impl/KeyValueStoreImpl.hpp)
