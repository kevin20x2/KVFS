/**
 * @file adaptor.cpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-17
 */
#include "adaptor.hpp"
#include "fs/inode.hpp"

namespace kvfs{

using namespace rocksdb;

const std::string RocksDBAdaptor ::default_DBpath = "/tmp/rocksdb";

RocksDBAdaptor :: RocksDBAdaptor():
    inited_(false){
    //default options

}

int RocksDBAdaptor::SetOptions(const Options & op){
    options_ = op;
    return 0;
}

Status RocksDBAdaptor :: Init(const std::string & path){

    Options option;
    option.IncreaseParallelism();
    //option.OptimizeLevelStyleCompaction();
    option.create_if_missing = true;
    if( path == "")
    {
        KVFS_LOG("RocksDBAdaptor : use default path\n");
        Status s = DB::Open(option,default_DBpath,&db_);
        KVFS_LOG("status : %s",s.ToString().c_str());
        assert(s.ok());
        return s;
    }
    else 
    {
        KVFS_LOG("RocksDBAdaptor : use path %s\n",path.c_str());
        Status s = DB::Open(option,path,&db_);
        KVFS_LOG(s.ToString().c_str());
        assert(s.ok());
        return s;
    }
}

Status RocksDBAdaptor :: Delete(const std::string & key )
{
    auto s = db_->Delete(WriteOptions(),key);
    return s;
}

Status RocksDBAdaptor :: Put(const Slice & key , const Slice & value)
{
    // default write options
    Status s = db_->Put(WriteOptions(),key,value);
    return s;
}
Status RocksDBAdaptor :: Get(const std::string & key,std::string * value)
{
    Status s = db_->Get(ReadOptions(),key,value);
    return s;
}

Status RocksDBAdaptor ::Sync()
{
    WriteOptions option;
    option.sync = true;
    Status s = db_->Put(option,"sync","");
    return s;
}

Status RocksDBAdaptor :: PutFileMetaData(const char * path){

}



}
