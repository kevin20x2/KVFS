/**
 * @file config.hpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-18
 */
#ifndef KVFS_CONFIG_HPP
#define KVFS_CONFIG_HPP

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <sys/stat.h>

namespace kvfs{

using kvfs_args = std::unordered_map <std::string, std::string >;

class KVFSConfig {

public :
    KVFSConfig();
    void Init(const kvfs_args & args);
    inline bool IsEmpty() { 
        return 0 == current_inode_size ;  }
    inline std::string GetDataDir()
    {
        return data_dir;
    }
    inline std::string GetMetaDir()
    {
        return meta_dir;
    }
    inline std::string GetMountDir()
    {
        return mount_dir;
    }
    inline size_t GetThresHold() 
    {
        return threshold;
    }

protected:

    // const path
    static const std::string config_file_name;

    // dirs 
    std::string meta_dir;
    std::string data_dir;
    std::string mount_dir;
    uint64_t current_inode_size;

    size_t threshold ;



};

}

#endif // KVFS_CONFIG_HPP
