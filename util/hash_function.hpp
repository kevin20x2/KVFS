/**
 * @file hash_function.hpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-18
 */

#ifndef KVFS_HASH_FUNCTION_HPP
#define KVFS_HASH_FUNCTION_HPP
#include <stdint.h>

namespace kvfs{

//    extern uint32_t crc32(const void * key ,int len );

    extern uint64_t murmur64(const void * key , int len , uint64_t seed);
}

#endif //KVFS_hash_fuction_hpp

