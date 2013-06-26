#include "Utils.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>


bool Utils::fileReadable(const std::string& path){
    return  access( path.c_str(), R_OK ) != -1 ;
}
bool Utils::fileWritable(const std::string& path){
    return  access( path.c_str(), W_OK ) != -1 ;
}
bool Utils::fileExecutable(const std::string& path){
    return  access( path.c_str(), X_OK ) != -1 ;
}
bool Utils::fileExist(const std::string& path){
    return  access( path.c_str(), F_OK ) != -1 ;
}
bool Utils::dirExist(const std::string& path){
    struct stat st;
    if(stat( path.c_str(),&st) == 0)
        return (st.st_mode & S_IFDIR != 0);
    return false;
}


long Utils::stringToLong(const std::string& in){
    long tmp=strtol(in.c_str(),NULL,0);
    return tmp;
}

bool Utils::fileToString(const std::string& path,std::string& out){
    if(!fileReadable(path)) return false;

    std::ifstream ifs(path);
    out.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return true;

}
bool Utils::stringToExistFile(const std::string& path,const std::string& in){

    if(!fileWritable(path)) return false;

    std::ofstream ofs(path);
    ofs<<(in);

    return true;
}
