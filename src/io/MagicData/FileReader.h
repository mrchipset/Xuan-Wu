#ifndef IO_MAGIC_FILE_READER_H
#define IO_MAGIC_FILE_READER_H

#include "api.h"


#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <string>
#include <vector>

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

    FileMetaHead fileMetaHead() const;

    bool getFileMetaInfo(MetaInfoBase* metaInfo);

    uint64_t getDataFrameCount() const;

    bool getDataSegmentHeadList(std::vector<SegmentItemHead>& segmentItemIndex) const;
    bool getDataSegmentHead(uint64_t id, SegmentItemHead* headInfo) const;
    bool getDataSegmentMetaInfo(uint64_t id, MetaInfoBase* metaInfo);
    bool getDataFrame(const SegmentItemHead& headInfo, DataFrameBase* dataFrame);
    bool getDataFrame(uint64_t id, DataFrameBase* dataFrame);

private:
    std::ifstream mFileHandle;
    FileMetaHead mFileMetaHead;
    FileMetaSignature mFileMetaSignature;
    FileCurrentStatus mFileStatus;
    std::vector<SegmentItemHead> mSegmentItemIndex;
    std::vector<uint64_t> mSegmentPosition;
    bool readFileMetaHead(FileMetaHead* fileMetaHead);
    bool readSegmentItemIndex(std::vector<SegmentItemHead>* segmentItemIndex,
                              std::vector<uint64_t>* segmentPosition);
protected:
    virtual bool checkSignature(FileMetaSignature* fileMetaSignature);

};

}   // end namespace XuanWu

#endif