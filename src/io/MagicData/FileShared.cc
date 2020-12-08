#include "FileShared.h"

using namespace XuanWu;

uint64_t MetaInfoBase::metaInfoSize() const
{
    return 0ULL;
}

void* MetaInfoBase::data() const
{
    return nullptr;
}


uint64_t DataFrameBase::serialHeadSize() const
{
    return 0ULL;
}

uint64_t DataFrameBase::dataSize() const
{
    return 0ULL;
}

void* DataFrameBase::headData() const
{
    return nullptr;
}

void* DataFrameBase::frameBodyData() const
{
    return nullptr;
}

void DataFrameBase::setDataPosition(uint64_t position)
{
    mSegementItemHead._DataPosition = position;
}

void DataFrameBase::setDataLength(uint64_t length)
{
    mSegementItemHead._DataLength = length;
}

const SegmentItemHead* DataFrameBase::segmentItemHead() const
{
    return &mSegementItemHead;
}