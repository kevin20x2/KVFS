/**
 * @file inode.hpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-17
 */
#ifndef KVFS_INODE_HPP
#define KVFS_INODE_HPP

#include <sys/stat.h>

#include <map>
#include <unordered_map>

#include "rocksdb/db.h"

#include "util/hash_function.hpp"



// inode definitions
namespace kvfs {

using  kvfs_stat_t = struct stat;
const size_t Inode_padding = 104;
using kvfs_meta_type = unsigned char  ;
const kvfs_meta_type KVFS_TYPE_DIR = 0;
const kvfs_meta_type KVFS_TYPE_SMALL_FILE = 1;
const kvfs_meta_type KVFS_TYPE_BIG_FILE = 2;
const uint64_t murmur64_hash_seed = 123;

// file sys treat file and dir the same way,
// so metadata type should in value struct
struct kvfs_inode_meta_key
{
    uint64_t parent_hash_id;
    uint64_t hash_id;

    static kvfs_inode_meta_key FromPath(const std::string & path)
    {
        kvfs_inode_meta_key ans;
        uint64_t hash_id = murmur64((const void *)path.c_str(),path.size(),murmur64_hash_seed);
        size_t pos = path.find_last_of("/");
        if(pos != std::string::npos)
        {
            std::string parent_dir = path.substr(0,pos);
            if(parent_dir == "") parent_dir = "/";
            ans.parent_hash_id = murmur64((const void *)parent_dir.c_str(),
                    parent_dir.size(),murmur64_hash_seed);
        }
        //uint64_t inode_id = current_index++;
        ans.hash_id = hash_id;
      //  ans.inode_id = inode_id;
      //  ans.type = type;
        return ans;
    }

    const std::string ToString() const 
    {
        return std::string((const char *)this,sizeof(kvfs_inode_meta_key));
    }
    rocksdb::Slice ToSlice() const {
        return rocksdb::Slice((const char *)this,sizeof(kvfs_inode_meta_key));
    }
};
// big file format 

struct kvfs_big_file_header
{
//    uint32_t data_size;
    uint16_t path_name_len;
    //char * path_name;
    //char * file_data;
};

//size fixed header
struct kvfs_inode_header
{
   kvfs_stat_t fstat; 
    char padding[Inode_padding];
    size_t path_name_len;
    union {
        uint32_t value_size;
        uint32_t big_file_number;
    };
    
    kvfs_meta_type type;
};


// header and value
struct kvfs_inode_value
{
    kvfs_inode_header header;
    char * path_name; 
    char * value;
    kvfs_inode_value() 
        : path_name(nullptr),
        value(nullptr){
            header.value_size = 0;
            header.path_name_len = 0;
        }

    //static kvfs_inode_value * FromBuffer(const char * buffer)
    //{
    //}

    std::string ToString() const 
    {
        auto tmp = this->ToBuffer();
        return std::string((const char *)tmp.second, tmp.first);
    }

    rocksdb::Slice ToSlice() const 
    {
        auto tmp = this->ToBuffer();
        return rocksdb::Slice(tmp.second,tmp.first);

    }

    // normally , path_name and value should be deleted
    // cz ToSlice() op generate new buffer
    virtual ~kvfs_inode_value()
    {
        if(nullptr != path_name )
            delete [] path_name;
        if(nullptr != value)
            delete [] value ;
    }
protected :
    std::pair <size_t , const char * > 
        ToBuffer() const 
        {
            size_t total_size = sizeof(header) + header.path_name_len+1+header.value_size;
            // TODO: use allocator ?
            // when to delete ?
            char * buffer = new char[total_size];
            char * offset = buffer;
            if(buffer != nullptr)
            {
                memcpy(offset, &header ,sizeof(header));
                offset += sizeof(header);
                memcpy(offset , path_name ,header.path_name_len);
                offset += header.path_name_len ;
                *offset  = '\0';
                offset ++;
                memcpy(offset ,value ,header.value_size);
            }
            return std::make_pair(total_size, buffer);
        }




};

enum kvfs_inode_mode {
    INODE_READ = 0 ,
    INODE_DELETE = 1 ,
    INODE_WRITE = 2,
};


// file handle use in memory 
// for fuse 
// global reference 
struct kvfs_file_handle
{
    kvfs_inode_meta_key key;
    int flags;
    kvfs_inode_mode mode;
    // file descriptor for big file
    int fd; 
    //size_t offset;
    //size_t data_size;
    std::string value;

    kvfs_file_handle(const char * path) :
        flags(0),fd(-1)//,offset(-1)
    {
        InsertHandle(std::string(path),this);
    }



    static kvfs_file_handle * GetHandle(const std::string & path);
    static bool InsertHandle(const std::string & path, kvfs_file_handle * handle);
    static bool DeleteHandle (const std::string & path);
    protected :
    // global hash map for query
    static std::unordered_map <std::string /*path*/, kvfs_file_handle * > handle_map ; 

};


#ifdef NDEBUG
#define KVFS_LOG(...)
#else 
class KVFSLogger {
    public:
    static KVFSLogger * GetInstance();
    void Log(const char *file_name,int line ,const char * data ,  ...);
    protected:
    KVFSLogger() :
        logfile_name("/root/kvfs/testdir/fs_log.log")
    {
        fp = fopen(logfile_name.c_str(),"w");
        assert(fp);

    };
    std::string logfile_name;
    FILE * fp;


    static KVFSLogger * instance_;
};

#define KVFS_LOG(...) KVFSLogger::GetInstance()->Log(__FILE__,__LINE__,__VA_ARGS__)

#endif // NDEBUG


};


#endif // KVFS_INODE_HPP
