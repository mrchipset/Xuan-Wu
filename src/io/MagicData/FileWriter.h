#ifndef IO_MAGIC_FILE_WRITER_H
#define IO_MAGIC_FILE_WRITER_H

#include "api.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <string>

#include "FileShared.h"

namespace XuanWu
{
/**
 * @brief DataFrame File Writer
 *
 */
class XUANWU_API FileWriter
{
public:
    FileWriter(const FileMetaHead& fileMetaHead);
    virtual ~FileWriter();
    bool writeMetaInfo(const MetaInfoBase* metaInfo);
    bool createNewFile(const std::string& path);
    bool close();
    FileMetaHead fileMetaHead() const;
    bool writeDataFrame(const DataFrameBase* head);

private:
    FileMetaHead mFileMetaHead;
    std::ofstream mFileHandle;
    FileCurrentStatus mCurrentStatus;

    bool createSegmentBlock(uint64_t segBlkSize);
    bool writeSerialHead(uint64_t segBlkOffset, uint64_t segBlkId, const DataFrameBase* head);
};



};


#endif

