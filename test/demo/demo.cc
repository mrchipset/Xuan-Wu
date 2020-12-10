#include "io/Process/Process.h"
#include "io/MagicData/FileWriter.h"
#include "io/MagicData/FileReader.h"

#include <iostream>
#include <chrono>

constexpr uint64_t buff_size = 16ULL * 1ULL;
class DataFrame : public XuanWu::DataFrameBase
{
public:
    typedef struct {
        uint64_t id;
        uint64_t value;
    } Data;

public:
    DataFrame() : XuanWu::DataFrameBase()
    {
        zero();
    }

    ~DataFrame()
    {
    }

    virtual const void* frameBodyData() const override
    {
        return &mData;
    }

    virtual uint64_t dataSize() const override
    {
        return sizeof(Data);
    }

    virtual bool deserialize(char* data, uint64_t size) override
    {
        if (size != buff_size) {
            return false;
        }
        memcpy(&mData, data, sizeof(Data));
        return true;
    }

    bool setData(uint64_t id, uint64_t value)
    {
        mData.id = id;
        mData.value = value;
        return true;
    }

    bool check(uint64_t id, uint64_t value)
    {
        return mData.id == id && mData.value == value;
    }

    void zero()
    {
        memset(&mData, 0x00, sizeof(Data));
    }

private:
    Data mData;
};

class FileMetaHead : public XuanWu::MetaInfoBase
{
public:
    typedef struct {
        uint64_t id;
        uint64_t mode;
        uint64_t serial;
    } FileMetaHeadData;

public:
    FileMetaHead(const FileMetaHeadData& data = FileMetaHeadData()) : MetaInfoBase(),
        mData(data)
    {

    }

    virtual uint64_t metaInfoSize() const
    {
        return sizeof(FileMetaHeadData);
    }

    virtual const void* data() const
    {
        return &mData;
    }

    virtual bool deserialize(char* data, uint64_t size)
    {
        if (size != metaInfoSize()) {
            return false;
        }
        memcpy(&mData, data, sizeof(FileMetaHeadData));
        return true;
    }

    void print()
    {
        std::cout << "id:" << mData.id << std::endl
                  << "mode:" << mData.mode << std::endl
                  << "serial:" << mData.serial << std::endl;
    }
private:
    FileMetaHeadData mData;
};

int main()
{
    auto time_start = std::chrono::system_clock::now();
    const uint64_t size = 1024ULL;
    try {
        XuanWu::FileMetaHead metaHead;
        memset(&metaHead, 0x00, sizeof(XuanWu::FileMetaHead));
        metaHead._FileVersionId = 0x0000000000000001ULL;
        metaHead._SegmentBlockSize = 8ULL;
        metaHead._SegmentItemMetaInfoSize = sizeof(FileMetaHead::FileMetaHeadData);
        metaHead._FileMetaInfoSize = sizeof(FileMetaHead::FileMetaHeadData);
        XuanWu::FileWriter writer(metaHead);
        writer.createNewFile("D:/tmp/test.dat");
        FileMetaHead::FileMetaHeadData data;
        data.id = 324ULL;
        data.mode = 3ULL;
        data.serial = 12394849323967489ULL;
        FileMetaHead fileMetaData(data);
        writer.writeMetaInfo(&fileMetaData);
        DataFrame* df = new DataFrame();
        for (uint64_t i = 0; i < size / df->dataSize(); ++i) {
            data.id = i;
            data.mode = 3ULL;
            data.serial = 12394849323967489ULL;
            auto head = std::make_shared<FileMetaHead>(data);
            df->setMetaInfo(head);
            df->setData(i, i * 2);
            writer.writeDataFrame(df);
        }
        delete df;
        writer.close();
        XuanWu::FileReader reader;
        reader.open("D:/tmp/test.dat");
        XuanWu::FileMetaHead metaHead2 = reader.fileMetaHead();
        int ret = memcmp(&metaHead2, &metaHead, sizeof(XuanWu::FileMetaHead));
        std::cout << "The result is:" << ret << std::endl;
        std::cout << "Dataframe count written:" << size / buff_size
                  << "\tread:" << reader.getDataFrameCount()
                  << std::endl;
        std::vector<XuanWu::SegmentItemHead> segmentItemIndex;
        reader.getDataSegmentHeadList(segmentItemIndex);
        FileMetaHead _FileMetaHead;
        reader.getFileMetaInfo(&_FileMetaHead);
        _FileMetaHead.print();
        for (int i = 0; i < segmentItemIndex.size(); ++i) {
            DataFrame d;
            d.zero();
            reader.getDataFrame(i, &d);
            if (!d.check(i, i * 2)) {
                std::cout << i << "\t"
                          << segmentItemIndex[i]._DataPosition << "\t"
                          << segmentItemIndex[i]._DataLength << "\t"
                          << std::endl;
            }

        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    auto time_end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Use Time: " << duration.count() / 1000.0 << "sec" << std::endl
              << "Written Speed: " << 1024ULL * 1000ULL / duration.count() << "MB/sec"
              << std::endl;
    return 0;
}