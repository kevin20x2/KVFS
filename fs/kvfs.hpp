/**
 * @file kvfs.hpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-18
 */

#ifndef KVFS_HPP
#define KVFS_HPP

#define FUSE_USE_VERSION 29
#include <fuse.h>
#include "fs/inode.hpp"
#include "fs/adaptor.hpp"
#include "fs/config.hpp"
#include "rocksdb/db.h"
#include "fs/config.hpp"

namespace kvfs{


class KVFS {
public :
    virtual ~KVFS(){
    }
    KVFS(const kvfs_args & arg);

    void * Init(struct fuse_conn_info * conn);

    void Destroy(void * data);

    int GetAttr(const char * path ,struct stat * statbuff);

    int Open(const char * path ,struct fuse_file_info * fi);

    int Read(const char * path,char * buf , size_t size ,off_t offset ,struct fuse_file_info * fi);

    int Write(const char * path , const char * buf,size_t size ,off_t offset ,struct fuse_file_info * fi);

    int Truncate(const char * path ,off_t offset);

    int Fsync(const char * path,int datasync ,struct fuse_file_info * fi);

    int Release(const char * path ,struct fuse_file_info * fi);

    int Readlink(const char * path ,char * buf,size_t size);

    int Symlink(const char * target , const char * path);

    int Unlink(const char * path);

    int MakeNode(const char * path,mode_t mode ,dev_t dev);

    int MakeDir(const char * path,mode_t mode);

    int OpenDir(const char * path,struct fuse_file_info *fi);

    int ReadDir(const char * path,void * buf ,fuse_fill_dir_t filler,
            off_t offset ,struct fuse_file_info * fi);

    int ReleaseDir(const char * path,struct fuse_file_info * fi);

    int RemoveDir(const char * path);

    int Rename(const char *new_path,const char * old_path);
    
    int Access(const char * path,int mask);

    int Chmod(const char * path , mode_t mode);
    
    int Chown(const char * path, uid_t uid,gid_t gid);

    int UpdateTimens(const char * path ,const struct timespec tv[2]);

protected:
    void InitStat(kvfs_stat_t & statbuf,uint64_t inode ,
            mode_t mode , dev_t dev);
   // int FillFileHandle(const char * path ,const  kvfs_inode_meta_key & key,   kvfs_file_handle * handle);
    int GetNewFileNo(const kvfs_inode_meta_key & key);

//    std::string GetBigDataFileName(const kvfs_inode_meta_key & key,const char * path);

    kvfs_file_handle * InitFileHandle(const char * path , struct fuse_file_info * fi,
            const kvfs_inode_meta_key & key , const std::string & value );
    int NewDir(const std::string & path , mode_t mode = 0);






    RocksDBAdaptor * db_;
    KVFSConfig * config_;
    kvfs_args args_;
 //   rocksdb :: DB * db;
    bool use_fuse;









};

}

#endif //KVFS_HPP
