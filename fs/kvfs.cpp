/**
 * @file kvfs.cpp
 * @brief 
 * @author kevin20x2@gmail.com
 * @version 1.0
 * @date 2018-12-18
 */

#include "fs/kvfs.hpp"
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <limits>
#include <algorithm>

namespace kvfs{

const std::string root_dir = "/";


static void LogStat(struct stat * statbuf,const char * path)
{
    KVFS_LOG("stat of [%s]:",path);
    KVFS_LOG("inode[%d] mode[%d] , uid[%d] , gid[%d] , size[%d]",
            statbuf->st_ino,
            statbuf->st_mode,statbuf->st_uid,
            statbuf->st_gid,statbuf->st_size);
    KVFS_LOG("atime [%d] mtime[%d] ctime[%d]\n",
            statbuf->st_atime,statbuf->st_mtime,
            statbuf->st_ctime
            );
}

static std::string GetParentDir(const std::string & path)
{
    size_t pos = path.find_last_of("/");
    if(pos != std::string ::npos)
    {
        std::string ans =  path.substr(0,pos);
        //KVFS_LOG("GetParentDir : %s ",ans.c_str());
        if( ans == "")
        {
            return root_dir;
        }
        return ans;
    }
    else 
    {
        KVFS_LOG("GetParentDir : not a valid path");
        exit(-1);
    }
}
static std::string GetFileName(const std::string  &path)
{
    size_t pos = path.find_last_of("/");
    if(pos != std::string ::npos)
    {
        std::string ans = path.substr(pos+1);
        return ans;
    }
    else 
    {
        KVFS_LOG("GetFileName : not a valid path");
        exit(-1);
    }
}


KVFS::KVFS(const kvfs_args & args) :
   args_(args), db_(nullptr),config_(nullptr),use_fuse(false)
{

    config_  = new KVFSConfig();
    config_->Init(args_);

    KVFS_LOG("init config..\n");

    KVFS_LOG("init rocksdb adaptor ..\n");
    db_ = new RocksDBAdaptor();
    db_->Init(config_->GetMetaDir());



}

int KVFS::NewDir(const std::string & path , mode_t mode)
{
    KVFS_LOG("NewDir : Create new dir : %s\n",path.c_str());
    kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(path);
    kvfs_inode_value value;
    // why ?
    std::string example_path = "/tmp/";
    kvfs_stat_t statbuf;
    //get stat as root stat
    lstat(example_path .c_str(), &statbuf);
    value.header.fstat = statbuf;
    if(mode == 0)
    {
        mode = mode | value.header.fstat.st_mode | S_IFDIR;
    }
    mode = mode | S_IFDIR;
    InitStat(value.header.fstat, 0 , mode , statbuf.st_dev);
    value.header.path_name_len = path.size();
    value.header.value_size = 0 ;
    value.header.type = KVFS_TYPE_DIR;

    LogStat(&value.header.fstat,path.c_str());

    // TODO : ugly !
    // dont forget to delete buffer ! 
    char * path_name_buffer = new char [root_dir.size()];
    strcpy(path_name_buffer,path.c_str());
    value.path_name = path_name_buffer;

    if(db_->Put(key.ToSlice(), value.ToSlice()).ok())
    {
        // path_name_buffer will managed by value ..
        // so here do nothing ..
    }
    return 0;

}

void * KVFS::Init(struct fuse_conn_info * conn)
{
    KVFS_LOG("kvfs init .. \n");
    if(conn != nullptr)
    {
        KVFS_LOG("use fuse .. true");
        use_fuse = true;
    }
    //
    kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(root_dir);
    std::string value;

    auto s = db_->Get(key.ToString(),&value);

    if(s.IsNotFound()) // empty filesystem 
    {
        KVFS_LOG("file system is empty .. \n create root inode .. ");
        NewDir(root_dir );
    }// empty()
    else 
    {
        KVFS_LOG("not empty ..  ");
    }
    return config_;
}

void KVFS::InitStat(kvfs_stat_t & statbuf,
        uint64_t inode , // local file system inode, useless ,always 0
        mode_t mode , dev_t dev)
{
    statbuf.st_ino = inode; 
    statbuf.st_mode = mode;
    statbuf.st_dev = dev;
    if(use_fuse )
    {
        statbuf.st_gid = fuse_get_context()->gid;
        statbuf.st_uid = fuse_get_context()->uid;
    }
    else 
    {
        statbuf.st_gid = 0;
        statbuf.st_uid = 0;
    }
    statbuf.st_size = 0;
    statbuf.st_blksize = 0;
    statbuf.st_blocks = 0;
    if ( S_ISREG(mode) )
    {
        statbuf.st_nlink = 1;
    }
    else 
    {
        statbuf.st_nlink = 2;
    }
    time_t now = time(nullptr);
    statbuf.st_atim.tv_nsec = 0 ;
    statbuf.st_mtim.tv_nsec = 0 ;
    statbuf.st_ctim.tv_sec = now ;
    statbuf.st_ctim.tv_nsec = 0 ;
}

/*
std::string KVFS::GetBigDataFileName(const kvfs_inode_meta_key & key , const char * path)
{
    std::string file_path = config_->GetDataDir() + "/" + std::to_string(key.hash_id);
    //const kvfs_inode_header * header =
      //  reinterpret_cast<const kvfs_inode_header*>(handle->value.data());
    //auto mode =  header ->fstat.st_mode;
    int count = 0;
    ssize_t ret = 0;
    while(true)
    {
        // file_name : hash_id + "_" + count ;
        std::string disk_file_path = file_path + "_" + std::to_string(count);
        int fd = open(disk_file_path.c_str(),O_RDONLY,0);
        if( fd >= 0)
        {
            kvfs_big_file_header file_header;
            ret = pread(fd,(void *)(&file_header),sizeof(file_header),0);
            if(ret >=0)
            {
                int offset = sizeof(file_header);
                char name_buffer[256];
                ret = pread(fd,name_buffer,file_header.path_name_len,offset);
                name_buffer[file_header.path_name_len] = '\0';
                offset += file_header.path_name_len;
                if(strcmp(path,name_buffer) == 0)
                {
                    return disk_file_path;
                }
            }
        }
        else 
        {
            KVFS_LOG("GetBigDataFileName : cant find file ..");
            return "";
        }
        close(fd);
        count ++;
    }

}

int KVFS::FillFileHandle(const char * path , 
        const kvfs_inode_meta_key & key, kvfs_file_handle * handle)
{
    // TODO : simplize by GetBigDataFileName
    // find file by key
    std::string file_path = config_->GetDataDir() + "/" + std::to_string(key.hash_id);
    const kvfs_inode_header * header = reinterpret_cast<const kvfs_inode_header*>(handle->value.data());
    auto mode =  header ->fstat.st_mode;
    int count = 0;
    ssize_t ret = 0;
    while(true)
    {
        // file_name : hash_id + "_" + count ;
        KVFS_LOG("Try to open file %s",(file_path +"_" + std::to_string(count)).c_str());
        int fd = open((file_path+"_" + std::to_string(count)).c_str(),O_RDONLY,mode );
        if( fd >= 0)
        {
            kvfs_big_file_header file_header;
            ret = pread(fd,(void *)(&file_header),sizeof(file_header),0);
            if(ret >=0)
            {
                int offset = sizeof(file_header);
                char name_buffer[256];
                ret = pread(fd,name_buffer,file_header.path_name_len,offset);
                name_buffer[file_header.path_name_len] = '\0';
                offset += file_header.path_name_len;
                KVFS_LOG("Fill File Handle : name_buffer %s\n",name_buffer);
                if(strcmp(path,name_buffer) == 0)
                {
                    handle->fd = fd;
                    handle->offset = offset;
                    return ret;
                }
            }
        }
        else 
        {
            KVFS_LOG("FillFileHandle : cant find file ..");
            return -1;
        }
        close(fd);
        count ++;
    }
        }
    return ret;
}
*/

void KVFS::Destroy(void * data)
{
}

kvfs_file_handle * KVFS::InitFileHandle(const char * path, struct fuse_file_info * fi
        ,const kvfs_inode_meta_key & key , const std::string & value )
{
       kvfs_file_handle * handle = new kvfs_file_handle(path); 
       handle->key = key;
       handle->value = value;
       handle->flags = fi->flags;
       if( (fi->flags & O_RDWR) >0 ||
               (fi->flags & O_WRONLY) > 0 || 
               (fi->flags & O_TRUNC) > 0)
       {
           handle->mode = INODE_WRITE;
       }
       else 
       {
           handle->mode = INODE_READ;
       }
       if(value != "") // file exists
       {
           const kvfs_inode_header * header
               = reinterpret_cast<const kvfs_inode_header *>(value.data());
           if(header->type == KVFS_TYPE_BIG_FILE) // for big file , fill fd 
           {
               std::string file_name = config_->GetDataDir() + "/" 
                   + std::to_string(key.hash_id) +"_" + std::to_string(header->big_file_number);
               handle->fd = open(file_name.c_str(),O_RDONLY,0);
               if(handle->fd <0)
               {
                   KVFS_LOG("Open: cant find data file %s",file_name.c_str());
               }

             //  FillFileHandle(path,key,handle);
           }
           else // small file or dir 
           {
               handle->fd = -1;
           }
       }
       else  // new file
       {

           kvfs_inode_value value ;
           value.header.type = KVFS_TYPE_SMALL_FILE;
           value.header.path_name_len = strlen(path);
           value.path_name = new char [value.header.path_name_len];
           strcpy(value.path_name,path);
           handle->fd = -1;
           handle->value = value.ToString();
           //mode_t default_mode = ;
           //InitStat(value.header.fstat,0,,);
           
       }
       fi->fh = reinterpret_cast <uint64_t >(handle);

       return handle;
}

// fill fi->fh as handle for write  and read 
// return open result
// consider use rwlock
int KVFS::Open(const char * path,struct fuse_file_info * fi)
{
    KVFS_LOG("Open : %s ,Flags: %d\n",path,fi->flags);

    std::string value ;
    KVFS_LOG("parentDir : %s \n",GetParentDir(path).c_str());
    kvfs_inode_meta_key parent_key = kvfs_inode_meta_key::FromPath(GetParentDir(path));

    // confirm that parent dir in db
    auto s = db_->Get(parent_key.ToString() ,&value);

    if( !s.ok()) // not in db
    {
        KVFS_LOG("Open : No such file or directory \n");
        return -errno;
    }
    else 
    {
        // 0. construct key
        kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(path);
        // 1 . find file 
        s = db_->Get(key.ToString(), &value);

        if(!s.ok()) // new file 
        {
            InitFileHandle(path,fi,key,"");
        }
        else 
        {
            // 2 . construct file handle 
            kvfs_file_handle * handle = InitFileHandle(path,fi,key,value);
        }
        return 0;
    }
}

int KVFS::GetAttr(const char * path ,struct stat * statbuff)
{
    KVFS_LOG("GetAttr : %s \n",path);
    // 0. construct key
    kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(path);
    // 1 . find file 
    std::string value ;
    auto s = db_->Get(key.ToString(), &value);

    if( !s.ok()) // not in db
    {
        KVFS_LOG("Open : No such file or directory \n");
        return -ENOENT;
    }
    else 
    {
        const kvfs_inode_header * header = reinterpret_cast<const kvfs_inode_header*>(value.data());

        //memcpy((void *)statbuff ,(const void *) &(header->fstat), sizeof(struct stat));
        *statbuff = header->fstat;
        LogStat(statbuff,path);
    }
    return 0;
}

static int GetInlineData(const std::string  &value , char * buf,off_t offset , size_t size)
{
    const kvfs_inode_header * header =reinterpret_cast <const kvfs_inode_header *>(value.data());
    if(header->type != KVFS_TYPE_SMALL_FILE)
    {
        KVFS_LOG("only small file contain inline data ..");
        return -1;
    }
    else 
    {
        KVFS_LOG("Inline Data :path_name_len %d",header->path_name_len);
        KVFS_LOG("value_size : %d ",header->value_size);
        size_t path_name_len = header -> path_name_len;
        uint32_t data_size = header -> value_size ;
        const char * buffer = value.data() + sizeof(kvfs_inode_header) + path_name_len +1+ offset;
        if( offset <= data_size)
        {
            size_t read_size = std::min(size ,(size_t)(data_size - offset));
            memcpy(buf,buffer , read_size);
            return read_size;
        }
        else 
        {
            return 0;
        }
        //return size;
    }


}
int KVFS::Read(const char * path,char * buf , size_t size ,off_t offset ,struct fuse_file_info * fi)
{
    KVFS_LOG("Read: %s size : %d , offset %d \n",path,size,offset);
    kvfs_file_handle * handle = reinterpret_cast <kvfs_file_handle *>(fi->fh);
    const kvfs_inode_header * header =
        reinterpret_cast <const kvfs_inode_header *>(handle->value.data());
    int ret;
    if(handle ->fd >=0  && header->type == KVFS_TYPE_BIG_FILE) // big file 
    {
        KVFS_LOG("Read : BigFile  %s",path);
        ret = pread(handle->fd, buf,size , offset );
    }
    else  if(header->type == KVFS_TYPE_SMALL_FILE && handle->fd <0)// small file 
    {
        KVFS_LOG("Read : SmallFile %s",path);
        ret = GetInlineData(handle->value , buf,offset,size);
    }
    else 
    {
        KVFS_LOG("Read : value error : %d %d ",header->type ,handle->fd);
        return -1;
    }
    return ret;
}

int KVFS::GetNewFileNo(const kvfs_inode_meta_key & key)
{
    int count = 0;
    std::string file_dir = config_->GetDataDir() +"/";
    while(true)
    {
        std::string file_path = file_dir + std::to_string(key.hash_id) + "_" + std::to_string(count);
        if( access(file_path.c_str(),F_OK) == -1) // file not exist , return
        {
            return count;
        }
        count ++ ;
    }
}

int KVFS::Write(const char * path , const char * buf,size_t size ,off_t offset ,struct fuse_file_info * fi)
{
    KVFS_LOG("Write : %s %lld %d\n",path,offset ,size);

    kvfs_file_handle * handle = reinterpret_cast<kvfs_file_handle *>(fi->fh);
    handle->mode = INODE_WRITE;
    const kvfs_inode_header * header = 
        reinterpret_cast<const kvfs_inode_header *> (handle->value.data());
    int ret = 0;
    bool has_larger_size = (header->fstat.st_size < offset + size);

    KVFS_LOG("Write :  %s has_lager_size : %d ",path,has_larger_size);

    if(handle -> fd >=0 ) //big file 
    {
        ret = pwrite(handle->fd , buf,size ,  + offset);
        kvfs_inode_header * mutable_header = const_cast<kvfs_inode_header *>(header);
        mutable_header->fstat.st_size = offset +size;
        auto s = db_->Put(handle->key.ToSlice(),handle->value);
        KVFS_LOG("Write: %s\n",s.ToString().c_str());
    }
    else 
    {
        // from small file to big file
        if(offset + size > config_->GetThresHold())
        {
            KVFS_LOG("Write :from small file to BigFile");
            //0 .copy inline data 
            char * buffer = new char[ offset + size];

            const char * inline_data = handle->value.data() + sizeof(kvfs_inode_header) + header->path_name_len;
            
            memcpy(buffer,inline_data,header->value_size);
            //1 . write data to buffer
            memcpy (buffer+ offset , buf,size);
            //2. write buffer to file
            int new_no = GetNewFileNo(handle->key);

            std::string new_file_dir = config_->GetDataDir() + "/" +
                std::to_string(handle->key.hash_id)+"_" + std::to_string(new_no);

            int fd = open(new_file_dir.c_str(),handle->flags | O_CREAT ,header->fstat.st_mode );
            ret = pwrite(fd,buffer,offset+size,0);
            //3 . delete tmp buffer
            if(ret > 0 )
            {
                delete [] buffer;
            }
            //4 . update inode 
            kvfs_inode_header * mutable_header = const_cast<kvfs_inode_header * >(header);
            mutable_header->type = KVFS_TYPE_BIG_FILE;
            mutable_header->fstat.st_size = offset+size;
            // when become big file ,value_size is number of data file
            mutable_header->big_file_number = new_no;
            handle->fd = fd;
            //handle->offset = base_offset;
            // direct write in db ?
            auto s = db_->Put(handle->key.ToSlice(),handle->value);
            ret = s.ok() ? size : -errno;
        }
        else //still small file 
        {
            KVFS_LOG("Write : from SmallFile to SmallFile");
            //kvfs_inode_value * pointer=  new kvfs_inode_value();
            kvfs_inode_value  new_value;// = *pointer;
            new_value .header = * header;
            new_value.header.value_size = has_larger_size ?offset+size:header->value_size;
            new_value.header.fstat .st_size = new_value.header.value_size;
            // copy path name , should it ?
            new_value.path_name = new char[header->path_name_len+1];
            memcpy(new_value.path_name , handle->value.data()+ sizeof(kvfs_inode_header),
                    header->path_name_len);
            // copy data
            new_value .value = new char[new_value.header.value_size];
            // old data 
            const char * inline_data = handle->value.data() + sizeof(kvfs_inode_header)
                + header->path_name_len;
            memcpy(new_value.value,inline_data,header->value_size);
            // new data
            memcpy(new_value.value + offset ,buf,size);
            // update fi
            handle->value = new_value .ToString();
            // udpate db
            auto s = db_->Put(handle->key.ToSlice(),handle->value);
            KVFS_LOG("Write: %s\n",s.ToString().c_str());
            ret = s.ok() ? size : -errno;
        }
    }
    return ret;
}

// TODO :no fuse_file_info args ,  rethink
int KVFS::Truncate(const char * path ,off_t offset)
{
    kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(path);
    std::string value ;
    auto s = db_->Get(key.ToString(),&value);
    if(!s.ok())
    {
        KVFS_LOG("no such file or directories .. ");
        return -1;
    }

    //kvfs_file_handle * handle = InitFileHandle(path,);


}

//  do file fsync or db sync
int KVFS::Fsync(const char * path,int datasync ,struct fuse_file_info * fi)
{
    KVFS_LOG("Fsync: %s\n",path);

    kvfs_file_handle * handle = reinterpret_cast <kvfs_file_handle *>(fi->fh);
    int ret = 0 ;
    if(handle->mode == INODE_WRITE)
    {
        if(handle->fd >=0 )
        {
            //big file
            ret = fsync(handle->fd);
        }
        if(datasync == 0)
        {
            auto s  = db_->Sync();
            if(s.ok())
                ret = 0;
            else ret = -1;
        }
    }
    return -ret;

}

int KVFS::Release(const char * path ,struct fuse_file_info * fi)
{
    KVFS_LOG("Relaease : path %s ",path);
    return 0;
}

int KVFS::Readlink(const char * path ,char * buf,size_t size)
{
    KVFS_LOG("Readlink : %s\n",path);
    return 0;
}

int KVFS::Symlink(const char * target , const char * path)
{
    KVFS_LOG("target : %s \n",target);
    return 0;
}

int KVFS::Unlink(const char * path)
{
    KVFS_LOG("unlink : %s \n",path);
    kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(path);
    std::string value ;
    auto s = db_->Get(key.ToString(),&value);

    if(!s.ok())
    {
        KVFS_LOG("unlink : no such file or directory \n");
        return -ENOENT;
    }
    kvfs_file_handle * handle = kvfs_file_handle::GetHandle(path);
    int ret = 0;

    if(nullptr != handle )
    {
        const kvfs_inode_header * header = reinterpret_cast <const kvfs_inode_header *>( handle->value.data());
        if(header->type == KVFS_TYPE_DIR) // unlink a dir 
        {
            KVFS_LOG("unlink : %s is a dir .. ");
            return -errno;
        }
        else if(header->type == KVFS_TYPE_BIG_FILE) // unlink a big file
        {
            close(handle->fd); // close fd
            //std::string disk_file_path = GetBigDataFileName(key,path);
            std::string disk_file_path = config_->GetDataDir() + "/" + 
                std::to_string(key.hash_id) + "_" + std::to_string(header->big_file_number);
            
            if(disk_file_path != "")
            {
                ret = unlink(disk_file_path .c_str());
            }
            //ret = unlink();
        }
        kvfs_file_handle::DeleteHandle(path); // delete in memory handle
    }

    db_->Delete(key.ToString()); // delete data in db
    return ret;
}

// 
int KVFS::MakeNode(const char * path,mode_t mode ,dev_t dev)
{
    KVFS_LOG("MakeNode : %s \n",path);
    kvfs_inode_meta_key key = kvfs_inode_meta_key ::FromPath(path);

    //size_t pos = std::string(path).find_last_of("/");
    //if(pos != std::string::npos)
    {
        std::string parent_path = GetParentDir(path);

        KVFS_LOG("parent_path : %s \n",parent_path .c_str());

        kvfs_inode_meta_key parent_key = kvfs_inode_meta_key :: FromPath(parent_path);
        std::string parent_value ;
        auto s = db_->Get(parent_key.ToString(),&parent_value);
        if(!s.ok())
        {
            KVFS_LOG("MakeNode : No such parent file or directory .");
            return -ENOENT;
        }
        kvfs_inode_value new_value ;
        new_value.header.path_name_len = strlen(path);
        new_value.header.type = KVFS_TYPE_SMALL_FILE;

        InitStat(new_value.header.fstat, 0 , mode | S_IFREG , dev);
        new_value.path_name = new char[new_value.header.path_name_len];
        strcpy(new_value.path_name,path);
        s  = db_->Put(key.ToSlice(),new_value.ToSlice());

        KVFS_LOG("MakeNode :Hash_id %lld, parent_id %lld",key.hash_id,key.parent_hash_id);
        if(s.ok())
            return 0;
        else return -errno;
        //new_value.header
    }
    //else 
    //{
      //  KVFS_LOG("MakeNode : path name error ..\n");
       // return -errno;
    //}
    return 0;
}

int KVFS::MakeDir(const char * path,mode_t mode)
{
    KVFS_LOG("MakeDir %s \n",path);
    std::string father_dir = GetParentDir(path);
    //size_t found = father_dir.find_last_of("/");
    //if(found != std::string::npos)
    {
      //  father_dir = father_dir.substr(0,found);
        kvfs_inode_meta_key key = kvfs_inode_meta_key ::FromPath(father_dir);
        std::string value;

        auto s = db_->Get(key.ToString(),&value);
        if(!s.ok())
        {
            KVFS_LOG("no such directory ..");
            errno = ENOENT;
            return -1;
        }
        else 
        {
            NewDir(path,mode | S_IFDIR);
            return 0;
            //kvfs_inode_meta_key new_key = kvfs_inode_meta_key::FromPath(path);
            //kvfs_inode_value value ;
            //auto s = db_->Put(new_key .ToString(),);
        }

    }


}

int KVFS::OpenDir(const char * path,struct fuse_file_info *fi)
{
    KVFS_LOG("OpenDir : %s \n",path);
    kvfs_inode_meta_key key = kvfs_inode_meta_key::FromPath(path);
    std::string value ;
    auto s = db_->Get(key.ToString(),&value);

    if(!s.ok())
    {
        KVFS_LOG("OpenDir: no such file or dictory.");
        return -1;
    }
    kvfs_file_handle * handle = new kvfs_file_handle(path);
    handle->fd = -1;
    handle->key = key;
    handle->flags = fi->flags;
    handle->mode = INODE_READ;
    handle->value = value;

    fi->fh = reinterpret_cast<uint64_t>(handle);
    return 0;
}

int KVFS::ReadDir(const char * path,void * buf ,fuse_fill_dir_t filler,
        off_t offset ,struct fuse_file_info * fi) 
{
    KVFS_LOG("ReadDir : %s\n",path);
    kvfs_file_handle * handle = reinterpret_cast <kvfs_file_handle *>(fi->fh);

    kvfs_inode_meta_key child_key_base;
    // child dir(file)'s parent_id is dir's hash_id 
    child_key_base.parent_hash_id = handle->key.hash_id;  
    child_key_base.hash_id = 0 ;   // smallest

    KVFS_LOG("ReadDir: child_parent_id %lld",child_key_base.parent_hash_id);
    int ret = 0;

    if(filler(buf,".",NULL,0) < 0)
    {
        KVFS_LOG("cannt read a diretory");
        return -errno;
    }
    if(filler(buf,"..",NULL,0) < 0)
    {
        KVFS_LOG("cannt read a directory ");
        return -errno;
    }

    auto it = db_->NewIterator();

    for(it->Seek(child_key_base.ToSlice());
            it->Valid() ;
            it->Next())
    {
        const kvfs_inode_meta_key * rkey = reinterpret_cast<const kvfs_inode_meta_key*>
            (it->key().data());
        if(rkey->parent_hash_id == handle->key.parent_hash_id &&
                rkey->hash_id == handle->key.hash_id)
        {
            continue;
        }
        if(rkey->parent_hash_id != child_key_base.parent_hash_id )
        {
            break;
        }
        KVFS_LOG("ReadDir: Hash_id %lld, parent_id %lld",rkey->hash_id,rkey->parent_hash_id);
        const char * name_buffer = it->value().data() + sizeof(kvfs_inode_header);
        std::string file_name = GetFileName(name_buffer);
        KVFS_LOG("ReadDir : file_name : %s\n",file_name.c_str());
        if(name_buffer[0] == '\0')
        {
            continue;
        }
        if( filler(buf,file_name.c_str(),NULL,0) < 0)
        {
            KVFS_LOG("ReadDir :  %s , fill error",file_name.c_str());
            ret = -1;
            break;
        }
    }
    delete it;
    return ret;
}

int KVFS::ReleaseDir(const char * path,struct fuse_file_info * fi)
{
    KVFS_LOG("ReleaseDir : %s\n",path);
    //DO Nothing 

}

int KVFS::RemoveDir(const char * path)
{
    KVFS_LOG("RemoveDir : %s\n",path);

    kvfs_inode_meta_key key = kvfs_inode_meta_key ::FromPath(path);
    std::string value;

    auto s = db_->Get(key.ToString(),&value);
    if(!s.ok())
    {
        KVFS_LOG("RemoveDir : No Such file or directory\n");
        return -errno;
    }
    int ret = 0;
    //get handle by path
    kvfs_file_handle * handle = kvfs_file_handle ::GetHandle(path);

    if(nullptr != handle )
    {
        auto s = db_->Delete(handle->key.ToString());
        if(!s.ok())
        {
            KVFS_LOG("RemoveDir : Delete dir error");
            return -errno;
        }
        kvfs_file_handle::DeleteHandle(path);
    }
    return ret;
}

int KVFS::Rename(const char *old_path,const char * new_path)
{
    KVFS_LOG("Rename: %s %s\n",old_path,new_path);
    // TODO: implent rename
    //
    return 0;
}

int KVFS::Access(const char * path,int mask)
{
    KVFS_LOG("Access : %s %d\n",path,mask);
    kvfs_inode_meta_key key = kvfs_inode_meta_key ::FromPath(path);
    std::string value ;

    auto s = db_->Get(key.ToString(),&value);


    return 0;
}


int KVFS::Chmod(const char * path , mode_t mode)
{
    kvfs_inode_meta_key key = kvfs_inode_meta_key ::FromPath(path);
    std::string value ;

    auto s  =db_->Get(key.ToString(),&value);
    if(!s.ok())
    {
        KVFS_LOG("Chmod : no such file or directory\n");
        return -errno;
    }
    int ret = 0;
    
    kvfs_file_handle * handle = kvfs_file_handle ::GetHandle(path);
    auto update_func = [&](std::string & value)  {
        const kvfs_inode_header * header = reinterpret_cast<const kvfs_inode_header *>(value.data());
        // remove const 
        kvfs_stat_t * mutable_stat = const_cast<kvfs_stat_t *>(&(header->fstat));
        mutable_stat ->st_mode = mode ;
        };
     
    if(nullptr != handle)
    {
       update_func(handle->value);
    }
    // update value
    update_func(value);
    // write db ? 
    db_->Put(key.ToSlice(),value);

    return ret;
}

int KVFS::Chown(const char * path, uid_t uid,gid_t gid)
{
    KVFS_LOG("Chown : %s\n",path);
    kvfs_inode_meta_key key  = kvfs_inode_meta_key ::FromPath(path);
    std::string value ;

    auto s = db_->Get(key.ToString() , &value);
    if(!s.ok())
    {
        KVFS_LOG("Chown : no such file or directory\n");
        return -errno;
    }
    int ret = 0;

    kvfs_file_handle * handle = kvfs_file_handle ::GetHandle(path);
    auto update_func = [&] (std::string & value) {
        const kvfs_inode_header * header = reinterpret_cast<const kvfs_inode_header *>(value.data());
        // remove const 
        kvfs_stat_t * mutable_stat = const_cast<kvfs_stat_t *>(&(header->fstat));
        mutable_stat ->st_uid = uid ;
        mutable_stat ->st_gid = gid ;
    };


    if(nullptr != handle)
    {
        update_func(handle->value);
    }
    update_func(value);
    db_->Put(key.ToSlice(),value);

    return ret;
}

int KVFS::UpdateTimens(const char * path ,const struct timespec tv[2])
{

    kvfs_inode_meta_key key = kvfs_inode_meta_key ::FromPath(path);
    std::string value ;

    auto s  =db_->Get(key.ToString(),&value);
    if(!s.ok())
    {
        KVFS_LOG("Chmod : no such file or directory\n");
        return -errno;
    }
    int ret = 0;
    
    kvfs_file_handle * handle = kvfs_file_handle ::GetHandle(path);
    auto update_func = [&](std::string & value)  {
        const kvfs_inode_header * header = reinterpret_cast<const kvfs_inode_header *>(value.data());
        // remove const 
        kvfs_stat_t * mutable_stat = const_cast<kvfs_stat_t *>(&(header->fstat));
        mutable_stat -> st_atim.tv_sec = tv[0].tv_sec ;
        mutable_stat -> st_atim.tv_nsec = tv[0].tv_nsec ;
        mutable_stat -> st_mtim.tv_sec = tv[1].tv_sec ;
        mutable_stat -> st_mtim.tv_nsec = tv[1].tv_nsec ;
        };
     
    if(nullptr != handle)
    {
       update_func(handle->value);
    }
    // update value
    update_func(value);
    // write db ? 
    db_->Put(key.ToSlice(),value);

    return ret;



}



}
