#ifndef IO_MAGIC_FILE_READER_H
#define IO_MAGIC_FILE_READER_H

#include "api.h"


#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <string>

#include "FileShared.h"


namespace XuanWu
{

class XUANWU_API FileReader
{
public:
    FileReader();
    virtual ~FileReader();

    bool open(const std::string& path);
    bool close();

    FileMetaHead FileMetaHead() const;

private:

};

}   // end namespace XuanWu

#endif