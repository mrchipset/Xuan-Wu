#include "FileReader.h"

using namespace XuanWu;
namespace fs = std::filesystem;

FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

bool FileReader::open(const std::string& path)
{
    close();
    if (!fs::exists(fs::path(path))) {
        std::cerr << R"(This path is not existed.)" << std::endl;
        return false;
    }
    mFileHandle.open(path, std::ios_base::in | std::ios_base::binary);
    if (!readFileMetaHead(&mFileMetaHead) ||
            !checkSignature(&mFileMetaSignature) ||
            !readSegmentItemIndex(&mSegmentItemIndex, &mSegmentPosition)) {
        mFileHandle.close();
    }
    return mFileHandle.is_open();
}

bool FileReader::close()
{
    if (mFileHandle.is_open()) {
        mFileHandle.close();
    }
    return true;
}

FileMetaHead FileReader::fileMetaHead() const
{
    return mFileMetaHead;
}

bool FileReader::getFileMetaInfo(MetaInfoBase* metaInfo)
{
    if (metaInfo->metaInfoSize() != mFileMetaHead._FileMetaInfoSize) {
        return false;
    }

    char* buff = new char[mFileMetaHead._FileMetaInfoSize];
    const uint64_t offset = sizeof(FileMetaHead) + sizeof(FileMetaSignature);
    try {
        mFileHandle.seekg(offset, std::ios_base::beg);
        mFileHandle.read(buff, mFileMetaHead._FileMetaInfoSize);
        // deserialize the data;
        metaInfo->deserialize(buff, mFileMetaHead._FileMetaInfoSize);
        delete[] buff;
    } catch (const std::exception& e) {
        delete[] buff;
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

uint64_t FileReader::getDataFrameCount() const
{
    return mFileMetaSignature._DataFrameCount;
}

bool FileReader::getDataSegmentHeadList(std::vector<SegmentItemHead>& segmentItemIndex) const
{
    segmentItemIndex = mSegmentItemIndex;
    return true;
}

bool FileReader::getDataSegmentHead(uint64_t id, SegmentItemHead* headInfo) const
{
    if (id < mSegmentItemIndex.size()) {
        headInfo->_DataLength = mSegmentItemIndex[id]._DataLength;
        headInfo->_DataPosition = mSegmentItemIndex[id]._DataPosition;
        return true;
    }
    return false;
}

bool FileReader::getDataSegmentMetaInfo(uint64_t id, MetaInfoBase* metaInfo)
{
    if (id >= mSegmentItemIndex.size()) {
        return false;
    }
    if (!mFileHandle.is_open()) {
        return false;
    }

    const uint64_t lSegmentNo = id / mFileMetaHead._SegmentBlockSize;
    const uint64_t lItemNo = id % (mFileMetaHead._SegmentBlockSize + 1ULL);
    const uint64_t lSegmentItemSize = sizeof(SegmentItemHead)
                                      + mFileMetaHead._SegmentItemMetaInfoSize;
    const uint64_t lOffset = mSegmentPosition.at(lSegmentNo) + lItemNo * lSegmentItemSize;
    char* buff = new char[mFileMetaHead._FileMetaInfoSize];
    try {
        mFileHandle.read(buff, mFileMetaHead._FileMetaInfoSize);
        metaInfo->deserialize(buff, mFileMetaHead._FileMetaInfoSize);
        delete[] buff;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        delete[] buff;
    }
    return true;
}

bool FileReader::getDataFrame(const SegmentItemHead& headInfo, DataFrameBase* dataFrame)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    auto item = std::find(std::begin(mSegmentItemIndex),
                          std::end(mSegmentItemIndex), headInfo);
    if (item == std::end(mSegmentItemIndex)) {
        std::cerr << "The data frame is not existed." << std::endl;
        return false;
    }

    uint64_t id = std::abs(std::distance(std::begin(mSegmentItemIndex), item));
    return getDataFrame(id, dataFrame);
}

bool FileReader::getDataFrame(uint64_t id, DataFrameBase* dataFrame)
{
    if (id >= mSegmentItemIndex.size()) {
        return false;
    }

    auto item = &mSegmentItemIndex.at(id);
    char* buff = new char[item->_DataLength];
    try {
        mFileHandle.seekg(item->_DataPosition);
        mFileHandle.read(buff, item->_DataLength);
        dataFrame->deserialize(buff, item->_DataLength);
        delete[] buff;
    } catch (const std::exception& e) {
        delete[] buff;
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

bool FileReader::readFileMetaHead(FileMetaHead* fileMetaHead)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    try {
        mFileHandle.seekg(0, std::ios_base::beg);
        mFileHandle.read(reinterpret_cast<char*>(fileMetaHead), sizeof(FileMetaHead));
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool FileReader::readSegmentItemIndex(std::vector<SegmentItemHead>* segmentItemIndex,
                                      std::vector<uint64_t>* segmentPosition)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    bool bRet = false;
    segmentItemIndex->resize(mFileMetaSignature._DataFrameCount);
    segmentPosition->clear();
    // calculate the first segment position
    const uint64_t lHeadPosition = sizeof(FileMetaHead)
                                   + sizeof(FileMetaSignature)
                                   + mFileMetaHead._FileMetaInfoSize;
    const uint64_t lSegmentItemSize = sizeof(SegmentItemHead)
                                      + mFileMetaHead._SegmentItemMetaInfoSize;
    const uint64_t lSegmentBlkSize = lSegmentItemSize
                                     * (mFileMetaHead._SegmentBlockSize + 1ULL);
    // read the segments
    char* buff = new char[lSegmentBlkSize];
    uint64_t lSegment = 0;
    uint64_t lDataFrameCount = 0;
    const SegmentItemHead* tmpSegmentItemHeadCurr = nullptr;
    segmentPosition->push_back(lHeadPosition);
    try {
        mFileHandle.seekg(lHeadPosition, std::ios_base::beg);
        while (lSegment < mFileMetaSignature._SegmentCount) {
            mFileHandle.read(buff, lSegmentBlkSize);
            for (uint64_t id = 0;
                    id < mFileMetaHead._SegmentBlockSize
                    && lDataFrameCount < mFileMetaSignature._DataFrameCount;
                    ++id, ++lDataFrameCount) {
                const uint64_t _offset = lSegmentItemSize * id;
                tmpSegmentItemHeadCurr = reinterpret_cast<SegmentItemHead*>(buff + _offset);
                memcpy(&segmentItemIndex->at(lDataFrameCount),
                       tmpSegmentItemHeadCurr, sizeof(SegmentItemHead));
            }

            // check read all valid data frame
            if (lDataFrameCount + 1 >= mFileMetaSignature._DataFrameCount) {
                break;
            }

            // seek to next segment
            const uint64_t offset = lSegmentItemSize * mFileMetaHead._SegmentBlockSize;
            const uint64_t lNextPosition = *reinterpret_cast<uint64_t*>(buff + offset);
            mFileHandle.seekg(lNextPosition, std::ios_base::beg);
            segmentPosition->push_back(lNextPosition);
        }
        delete[] buff;
        bRet = segmentPosition->size() == mFileMetaSignature._SegmentCount;
    } catch (const std::exception& e) {
        delete[] buff;
        std::cerr << e.what() << std::endl;
        return false;
    }

    return bRet;
}

bool FileReader::checkSignature(FileMetaSignature* fileMetaSignature)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    // read in the signature
    try {
        mFileHandle.seekg(sizeof(FileMetaHead), std::ios_base::beg);
        mFileHandle.read(reinterpret_cast<char*>(fileMetaSignature),
                         sizeof(FileMetaSignature));
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    // TODO check the signature
    //
    return true;
}