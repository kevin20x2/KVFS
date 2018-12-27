/**
 * @file adaptor.hpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-17
 */
#ifndef KVFS_ADAPTOR_HPP
#define KVFS_ADAPTOR_HPP

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include <string>

namespace kvfs {
    using namespace rocksdb;

class RocksDBAdaptor {
public:

    Status Put(const Slice & key,  const Slice & value ); 

    //Status Get(const Slice & key);
    Status Get(const std::string & key,std::string * value);

    Status Delete(const std::string & key );

    Status PutFileMetaData(const char * path);

    Status Sync();
//

    int SetOptions(const Options & op);
    Status Init(const std::string & path = "");

    inline Iterator * NewIterator() { return db_->NewIterator(ReadOptions()); }

    RocksDBAdaptor();

protected:
    bool inited_;
    DB * db_;
    Options options_;
    static const std::string default_DBpath;

};


}
#endif
