#include "FileReader.h"

using namespace XuanWu;

FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

bool FileReader::open(const std::string& path)
{
    return true;
}

bool FileReader::close()
{
    return true;
}

FileMetaHead FileReader::fileMetaHead() const
{
    return mFileMetaHead;
}