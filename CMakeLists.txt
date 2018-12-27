cmake_minimum_required(VERSION 3.9)
project(KVFS)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSOIONS OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig)

pkg_check_modules(fuse REQUIRED fuse)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${fuse_CFLAGS_OTHER}")

include_directories(.)
include_directories(/root/rocksdb/include )
include_directories(${fuse_INCLUDE_DIR})


aux_source_directory(fs fs_srcs)

aux_source_directory(util util_srcs)

#link_directories(./leveldb)
link_directories(/root/rocksdb)

add_executable(fs_main fs_main.cpp ${fs_srcs} ${util_srcs})

add_executable(test test.cpp)
message(STATUS ${fuse_LIBRARIES})

target_link_libraries(fs_main librocksdb.a ${fuse_LIBRARIES} pthread )
