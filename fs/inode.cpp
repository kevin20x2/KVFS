#include "fs/inode.hpp"
#include <iostream>




namespace kvfs{

//uint64_t kvfs_inode_meta_key ::current_index = 0;


std::unordered_map <std::string ,kvfs_file_handle *> kvfs_file_handle :: handle_map = 
    std::unordered_map <std::string ,kvfs_file_handle *>();

kvfs_file_handle * kvfs_file_handle::GetHandle(const std::string & path)
{
    if(handle_map .find(path) != handle_map.end())
        return handle_map.at(path);
    else return nullptr;
}
bool kvfs_file_handle :: InsertHandle(const std::string & path , kvfs_file_handle * handle)
{
    if(handle_map.find(path) != handle_map.end())
    {
        return false;
    }
    else 
    {
        handle_map[path] = handle;
        return true;
    }
}
bool kvfs_file_handle :: DeleteHandle(const std::string & path)
{
    auto it = handle_map .find(path) ;
    if(it!= handle_map.end())
    {
        delete it->second;
        handle_map.erase(it);
        return true;
    }
    else return false;

}


#ifndef NDEBUG
KVFSLogger * KVFSLogger :: instance_ = nullptr;

KVFSLogger * KVFSLogger :: GetInstance()
{
    if(instance_ == nullptr)
    {
        instance_ = new KVFSLogger();
    }
    return instance_;
}


void KVFSLogger :: Log(const char * file_name ,int line, const char * data,  ...)
{
    va_list ap;
    va_start(ap,data);
    //std::cout << data << std::endl;
    fprintf(fp,"[%s : %d]- ",file_name ,line);
    vfprintf(fp,data,ap);
    int len = strlen(data);
    if(data[len-1] != '\n')
        fprintf(fp,"\n");
    fflush(fp);
}
#endif // NDEBUG

}

