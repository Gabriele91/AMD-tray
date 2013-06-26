#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>

class Utils{

public:

    static bool fileReadable(const std::string& path);
    static bool fileWritable(const std::string& path);
    static bool fileExecutable(const std::string& path);
    static bool fileExist(const std::string& path);
    static bool dirExist(const std::string& path);

    //cast strings
    template<class T>
    static std::string toString(T v){
       std::stringstream ss;
       ss << v;
       return ss.str();
    }
    static long stringToLong(const std::string& in);
    static bool fileToString(const std::string& path,std::string& out);
    static bool stringToExistFile(const std::string& path,const std::string& in);
};

#endif
