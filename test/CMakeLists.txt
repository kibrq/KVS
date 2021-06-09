cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fsanitize=address -fsanitize=undefined")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(KVSTest testAll.cpp testTypedRepository.cpp ../src/Repository.cpp ../src/RepositoryFactory.cpp)

include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(KVSTest ${GTEST_LIBRARIES} Threads::Threads)

add_Test(Test KVSTest)
