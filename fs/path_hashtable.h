#ifndef PATH_HASH_TABLE_H
#define PATH_HASH_TABLE_H
#include <unordered_map> 
#include <leveldb/status.h>

namespace kvfs{

struct Inode_handle
{
    uint_64 hash_id_;
    uint_64 secondary_hash_id_;

};

class PathHashTable{
public :
    virtual ~PathHashTable(){
    }
    leveldb::Status AddDirectory(const std::string & dir_name );


    


protected :
    std::unordered_map <std::string , Inode_handle > inter_hash_map_;


};

}
#endif
