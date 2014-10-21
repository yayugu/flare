CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(flare CXX)

SET(CMAKE_C_COMPILER   /usr/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -lz")
set(CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -lz")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lz")


## flare

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/lib")

file(GLOB LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/*.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/storage_kch.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/zookeeper_coordinator.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/zookeeper_lock.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/mysql_replication.cc)



file(GLOB FLAREI_SRC ${PROJECT_SOURCE_DIR}/src/flarei/*.cc)
add_executable(flarei ${FLAREI_SRC} ${LIB_SRC})
set_target_properties(flarei PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB FLARED_SRC ${PROJECT_SOURCE_DIR}/src/flared/*.cc)
add_executable(flared ${FLARED_SRC} ${LIB_SRC})
set_target_properties(flared PROPERTIES LINKER_LANGUAGE CXX)



## librariesa
#set(Boost_USE_STATIC_LIBS ON)
#set(BOOST_ROOT /Users/yuya.yaguchi/works/boost)
#set(BOOST_ROOT /usr/local/Cellar/boost/1.56.0)
find_package(Boost COMPONENTS program_options regex serialization thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
#include_directories(/Users/yuya.yaguchi/works/boost/include)
#include_directories(/usr/local/Cellar/boost/1.56.0/include)

#set(Zlib_DIR /usr/local/Cellar/lzlib/1.6/)
#find_package(Zlib REQUIRED)
#include_directories(/usr/local/Cellar/lzlib/1.6/include)

target_link_libraries(flarei
  ${Boost_LIBRARIES}
  /usr/local/lib/libtokyocabinet.dylib
  /usr/local/lib/libuuid.a
  /usr/local/lib/libhashkit.a
)
target_link_libraries(flared
  ${Boost_LIBRARIES}
  /usr/local/lib/libtokyocabinet.dylib
  /usr/local/lib/libuuid.a
  /usr/local/lib/libhashkit.a
)


## test

enable_testing()
include_directories("${PROJECT_SOURCE_DIR}/test/lib")

file(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/test/lib/*.cc)
list(REMOVE_ITEM TEST_SRC ${PROJECT_SOURCE_DIR}/test/lib/test_zookeeper_coordinator.cc)
list(REMOVE_ITEM TEST_SRC ${PROJECT_SOURCE_DIR}/test/lib/test_zookeeper_lock.cc)
list(REMOVE_ITEM TEST_SRC ${PROJECT_SOURCE_DIR}/test/lib/test_storage_kch.cc)

include_directories(/usr/local/Cellar/cutter/1.2.4/include/cutter)
link_directories(/usr/local/lib)
add_library(cutter_test
  MODULE
  ${TEST_SRC}
  ${LIB_SRC}
)
target_link_libraries(cutter_test
  ${Boost_LIBRARIES}
  /usr/local/lib/libtokyocabinet.dylib
  /usr/local/lib/libuuid.a
  /usr/local/lib/libhashkit.a
  cutter
  cppcutter
)

