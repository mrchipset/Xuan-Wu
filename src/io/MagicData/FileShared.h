#ifndef IO_MAGIC_FILESHARED_H
#define IO_MAGIC_FILESHARED_H

#include "api.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <string>
#include <memory>

namespace XuanWu
{

#pragma pack(push)
#pragma pack(8)
/**
 * @brief The struct defines the basic file info
 *
 */
typedef struct _FileMetaHead {
public:
    uint64_t _SegmentBlockSize;  ///< The size of the serial header block
    // Magic Serial No 128Bit
    uint64_t _MagicSerialNoLower64Bit;
    uint64_t _MagicSerialNoUpper64Bit;
    uint64_t _SerialHeadId;   ///< Reserved for parsing flag
    uint64_t _SerialBodyId; ///< Serial Body Id.
    uint64_t _FileVersionId;    ///< The file IO version id, write by the worker class;
    uint64_t _FileMetaInfoId;   ///< The meta info at the top of the file
    uint64_t _FileMetaInfoSize;
    uint64_t _SegmentItemMetaInfoId;   ///< meta info at the segment head
    uint64_t _SegmentItemMetaInfoSize;
} FileMetaHead;

typedef struct _FileMetaSignature {
    uint64_t _SegmentCount;
    uint64_t _DataFrameCount;
    uint64_t _Signature;
    uint64_t _Placeholder;  ///< placeholder in order to align the byte
} FileMetaSignature;

typedef struct _FileCurrentStatus {
public:
    uint64_t _SegmentBlkId; ///< The offset id of the segment
    uint64_t _SegmentBlkOffset; ///< The file position offset of the file
    uint64_t _FileCurrentSize;  ///< The end position of the file.
    uint64_t _FileCurrentPos;
} FileCurrentStatus;

typedef struct _SegmentItemHead {
public:
    uint64_t _DataPosition;
    uint64_t _DataLength;

    bool operator==(const _SegmentItemHead& other)
    {
        return other._DataLength == this->_DataLength
               && other._DataPosition == this->_DataPosition;
    }
} SegmentItemHead;

#pragma pack(pop)
/**
 * @brief The base class for meta information
 *
 */
class XUANWU_API MetaInfoBase
{
public:
    MetaInfoBase();
    virtual ~MetaInfoBase();
    virtual uint64_t metaInfoSize() const;
    virtual void* data() const;
    virtual bool deserialize(char* data, uint64_t size);
};

/**
 * @brief The base class of the data frame
 *
 */
class XUANWU_API DataFrameBase
{
public:
    DataFrameBase();
    virtual ~DataFrameBase();
    virtual uint64_t serialHeadSize() const;
    virtual uint64_t dataSize() const;
    virtual void* headData() const;
    virtual void* frameBodyData() const;
    virtual bool deserialize(char* data, uint64_t size);

    void setDataPosition(uint64_t position);
    void setDataLength(uint64_t length);
    void setMetaInfo(const std::shared_ptr<MetaInfoBase>& metaInfoPtr);
    const std::shared_ptr<MetaInfoBase>& metaInfo() const;
    const SegmentItemHead* segmentItemHead() const;
protected:
    SegmentItemHead mSegementItemHead;
    std::shared_ptr<MetaInfoBase> mMetaInfoPtr;
};

}   //end namespace XuanWu


#endif