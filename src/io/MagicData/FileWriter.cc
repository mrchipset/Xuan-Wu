#include "FileWriter.h"


using namespace XuanWu;
namespace fs = std::filesystem;


inline static bool GetCurrentStatus(std::ofstream& handle,
                                    FileCurrentStatus* const status)
{
    if (!handle.is_open() || status == nullptr) {
        return false;
    }

    try {
        status->_FileCurrentPos = handle.tellp();
        handle.seekp(0, std::ios_base::end);
        status->_FileCurrentSize = handle.tellp();
        handle.seekp(status->_FileCurrentPos, std::ios_base::beg);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

inline static bool SetCurrentStatus(std::ofstream& handle,
                                    FileCurrentStatus* const status)
{
    if (!handle.is_open() || status == nullptr) {
        return false;
    }

    try {
        handle.seekp(0, std::ios_base::end);
        status->_FileCurrentSize = handle.tellp();
        handle.seekp(status->_FileCurrentPos, std::ios_base::beg);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

FileWriter::FileWriter(const FileMetaHead& fileMetaHead) :
    mFileMetaHead(fileMetaHead),
    mFileMetaSignature({0ULL, 0ULL})
{

}

FileWriter::~FileWriter()
{
    close();
}

bool FileWriter::createNewFile(const std::string& path)
{
    close();
    // initialize the status
    memset(&mFileMetaSignature, 0x00, sizeof(FileMetaSignature));
    memset(&mCurrentStatus, 0x00, sizeof(FileCurrentStatus));

    try {
        // check the file existance.
        if (fs::exists(fs::path(path))) {
            std::cerr << R"(The path is already existed.)" << std::endl;
            fs::remove(path);
        }

        mFileHandle.open(path, std::ios_base::out | std::ios_base::binary);
        if (!createFileHead(mFileMetaHead, mFileMetaSignature)) {
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << R"(Create new file failure.)" << std::endl;
        std::cerr << e.what() << std::endl;
        return false;
    }
    return mFileHandle.is_open();
}

bool FileWriter::close()
{
    try {
        if (mFileHandle.is_open()) {
            bool ret = writeFileMetaSignature(mFileMetaSignature);
            mFileHandle.close();
            if (!ret) {
                std::cerr << "Write the meta signature error" << std::endl;
                return false;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool FileWriter::writeMetaInfo(const MetaInfoBase* metaInfo)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    if (metaInfo->metaInfoSize() != mFileMetaHead._FileMetaInfoSize) {
        std::cerr << R"(The meta info szie is not matched.)" << std::endl;
        return false;
    }
    if (!GetCurrentStatus(mFileHandle, &mCurrentStatus)) {
        std::cerr << R"(Get the status error. Please check the file.)" << std::endl;
        return false;
    }
    try {
        uint64_t position = sizeof(FileMetaHead) + sizeof(FileMetaSignature);
        mFileHandle.seekp(position, std::ios_base::beg);
        mFileHandle.write(reinterpret_cast<const char*>(metaInfo->data()),
                          metaInfo->metaInfoSize());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }

    if (!SetCurrentStatus(mFileHandle, &mCurrentStatus)) {
        std::cerr << R"(Set the status error. Please check the file.)" << std::endl;
        return false;
    }
    return true;
}

bool FileWriter::writeDataFrame(DataFrameBase* dataFrame)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    // check the segment space
    if (mCurrentStatus._SegmentBlkId == mFileMetaHead._SegmentBlockSize //current segment is full
            || mFileMetaSignature._SegmentCount == 0) { //first segment
        if (!createSegmentBlock(mFileMetaHead._SegmentBlockSize,
                                dataFrame->serialHeadSize() + sizeof(SegmentItemHead))) {
            return false;
        }
    }

    if (!GetCurrentStatus(mFileHandle, &mCurrentStatus)) {
        return false;
    }

    dataFrame->setDataPosition(mCurrentStatus._FileCurrentSize);
    dataFrame->setDataLength(dataFrame->dataSize());

    // write the serial head
    if (!writeSerialHead(mCurrentStatus._SegmentBlkOffset,
                         mCurrentStatus._SegmentBlkId, dataFrame)) {
        std::cerr << "Write the serial head error" << std::endl;
        return false;
    }
    mCurrentStatus._SegmentBlkId++;
    // write the data frame
    try {
        mFileHandle.seekp(0, std::ios_base::end);
        mFileHandle.write(reinterpret_cast<const char*>(dataFrame->frameBodyData()),
                          dataFrame->dataSize());
        mFileMetaSignature._DataFrameCount++;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return true;
}

bool FileWriter::createFileHead(const FileMetaHead& metaHead,
                                const FileMetaSignature& metaSignature)
{
    if (!mFileHandle.is_open()) {
        return false;
    }
    char* pData = new char[metaHead._FileMetaInfoSize];
    try {
        mFileHandle.seekp(0);
        mFileHandle.write(reinterpret_cast<const char*>(&metaHead),
                          sizeof(metaHead));
        mFileHandle.write(reinterpret_cast<const char*>(&metaSignature),
                          sizeof(metaSignature));
        mFileHandle.write(pData, metaHead._FileMetaInfoSize);
        delete[] pData;
    } catch (const std::exception& e) {
        delete[] pData;
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool FileWriter::createSegmentBlock(uint64_t segBlkSize, uint64_t segItemSize)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    char* pData = new char[segItemSize];
    memset(pData, 0x00, sizeof(char) * segItemSize);
    try {
        if (mFileMetaSignature._SegmentCount != 0) { // check not first Segment
            // write the next segment position
            GetCurrentStatus(mFileHandle, &mCurrentStatus); // get the file size
            const uint64_t position = mCurrentStatus._SegmentBlkOffset
                                      + segBlkSize * segItemSize;
            mFileHandle.seekp(position, std::ios_base::beg);
            mFileHandle.write(reinterpret_cast<char*>(&mCurrentStatus._FileCurrentSize),
                              sizeof(mCurrentStatus._FileCurrentSize)); // write the link position
        }

        //write the block
        GetCurrentStatus(mFileHandle, &mCurrentStatus); // get the file size
        mFileHandle.seekp(0, std::ios_base::end);
        for (int i = 0; i <= segBlkSize; ++i) {
            mFileHandle.write(pData, sizeof(char) * segItemSize);
        }

        // update the status
        mCurrentStatus._SegmentBlkOffset = mCurrentStatus._FileCurrentSize;
        mCurrentStatus._SegmentBlkId = 0;
        mFileMetaSignature._SegmentCount++;
        delete[] pData;
    } catch (const std::exception& e) {
        delete[] pData;
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool FileWriter::writeSerialHead(uint64_t segBlkOffset, uint64_t segBlkId, const DataFrameBase* dataFrame)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    try {
        GetCurrentStatus(mFileHandle, &mCurrentStatus);
        const uint64_t headSize = dataFrame->serialHeadSize() + sizeof(SegmentItemHead);
        const uint64_t position = mCurrentStatus._SegmentBlkOffset + segBlkId * headSize;
        //write the data position and size
        mFileHandle.seekp(position, std::ios_base::beg);
        mFileHandle.write(reinterpret_cast<const char*>(dataFrame->segmentItemHead()),
                          sizeof(SegmentItemHead));
        mFileHandle.write(reinterpret_cast<const char*>(dataFrame->headData()),
                          dataFrame->serialHeadSize());
        SetCurrentStatus(mFileHandle, &mCurrentStatus);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;

}

bool FileWriter::writeFileMetaSignature(const FileMetaSignature& metaSignature)
{
    if (!mFileHandle.is_open()) {
        return false;
    }

    try {
        GetCurrentStatus(mFileHandle, &mCurrentStatus);
        const uint64_t position = sizeof(FileMetaHead);
        mFileHandle.seekp(position, std::ios_base::beg);
        mFileHandle.write(reinterpret_cast<const char*>(&metaSignature),
                          sizeof(metaSignature));
        SetCurrentStatus(mFileHandle, &mCurrentStatus);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return true;
}