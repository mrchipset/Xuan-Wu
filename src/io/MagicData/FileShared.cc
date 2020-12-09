#include "FileShared.h"

using namespace XuanWu;

MetaInfoBase::MetaInfoBase()
{

}

MetaInfoBase::~MetaInfoBase()
{

}

uint64_t MetaInfoBase::metaInfoSize() const
{
    return 0ULL;
}

void* MetaInfoBase::data() const
{
    return nullptr;
}

bool MetaInfoBase::deserialize(char* data, uint64_t size)
{
    return true;
}

DataFrameBase::DataFrameBase() :
    mMetaInfoPtr(nullptr)
{

}

DataFrameBase::~DataFrameBase()
{

}

uint64_t DataFrameBase::serialHeadSize() const
{
    if (!mMetaInfoPtr) {
        return 0ULL;
    }
    return mMetaInfoPtr->metaInfoSize();
}

uint64_t DataFrameBase::dataSize() const
{
    return 0ULL;
}

void* DataFrameBase::headData() const
{
    if (!mMetaInfoPtr) {
        return nullptr;
    }
    return mMetaInfoPtr->data();
}

void* DataFrameBase::frameBodyData() const
{
    return nullptr;
}

bool DataFrameBase::deserialize(char* data, uint64_t size)
{
    return true;
}

void DataFrameBase::setDataPosition(uint64_t position)
{
    mSegementItemHead._DataPosition = position;
}

void DataFrameBase::setDataLength(uint64_t length)
{
    mSegementItemHead._DataLength = length;
}

void DataFrameBase::setMetaInfo(const std::shared_ptr<MetaInfoBase>& metaInfoPtr)
{
    mMetaInfoPtr = metaInfoPtr;
}

const std::shared_ptr<MetaInfoBase>& DataFrameBase::metaInfo() const
{
    return mMetaInfoPtr;
}


const SegmentItemHead* DataFrameBase::segmentItemHead() const
{
    return &mSegementItemHead;
}
