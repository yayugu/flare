CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(flare CXX)

SET(CMAKE_C_COMPILER   /usr/local/bin/gcc-4.8)
SET(CMAKE_CXX_COMPILER /usr/local/bin/g++-4.8)

## flare

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/lib")

file(GLOB LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/*.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/storage_kch.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/zookeeper_coordinator.cc)
list(REMOVE_ITEM LIB_SRC ${PROJECT_SOURCE_DIR}/src/lib/zookeeper_lock.cc)


file(GLOB FLAREI_SRC ${PROJECT_SOURCE_DIR}/src/flarei/*.cc)
add_executable(flarei ${FLAREI_SRC} ${LIB_SRC})
set_target_properties(flarei PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB FLARED_SRC ${PROJECT_SOURCE_DIR}/src/flared/*.cc)
add_executable(flared ${FLARED_SRC} ${LIB_SRC})
set_target_properties(flared PROPERTIES LINKER_LANGUAGE CXX)



## librariesa
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT /Users/yuya.yaguchi/works/boost)
find_package(Boost COMPONENTS program_options regex serialization thread REQUIRED)
include_directories(/Users/yuya.yaguchi/works/boost/include)

#set(Zlib_DIR /usr/local/Cellar/lzlib/1.6/)
#find_package(Zlib REQUIRED)
#include_directories(/usr/local/Cellar/lzlib/1.6/include)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lz")

target_link_libraries(flarei
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  /usr/local/lib/libtokyocabinet.dylib
  /usr/local/lib/libuuid.a
  /usr/local/lib/libhashkit.a
)
target_link_libraries(flared
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  /usr/local/lib/libtokyocabinet.dylib
  /usr/local/lib/libuuid.a
  /usr/local/lib/libhashkit.a
)