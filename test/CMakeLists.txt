cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fsanitize=address -fsanitize=undefined")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(KVSTest
        ../src/Repository.cpp ../src/RepositoryFactory.cpp
        testAll.cpp
        TestUtils.hpp TestUtilsImpl.hpp
        testLazyUnalignedIntArray.cpp
        testTypedRepository.cpp testFreePositionsManager.cpp testStorage.cpp testLog.cpp testIndex.cpp StubFilter.hpp testBloomFilter.cpp )

include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(KVSTest ${GTEST_LIBRARIES} Threads::Threads)

add_Test(Test KVSTest)
