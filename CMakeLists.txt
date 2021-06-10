cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER g++-10)

project(KVS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wconversion")

include_directories(include/)

enable_testing()

add_subdirectory(test)
add_subdirectory(benchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(KVS
        src/Main.cpp
        src/RepositoryFactory.cpp
        src/Repository.cpp
        include/KeyValue.hpp
        include/KeyValueStore.hpp
        include/RepositoryFactory.hpp
        include/Repository.hpp
        include/Serializer.hpp
        include/SerializerSized.hpp
        include/TypedRepository.hpp
        include/ValueSerializer.hpp
        include/LazyUnalignedArray.hpp
        include/LazyUnalignedKeyArray.hpp
        include/LazyUnalignedIntArray.hpp
        include/LimitedUnsignedInt.hpp
        include/TableBlock.hpp
        include/TableBlockSerializer.hpp
        include/Storage.hpp
        include/FreePositionsManager.hpp)
