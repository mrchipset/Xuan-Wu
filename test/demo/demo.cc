#include "io/Process/Process.h"
#include "io/MagicData/FileWriter.h"
#include "io/MagicData/FileReader.h"

#include <iostream>
#include <chrono>

constexpr uint64_t buff_size = 16ULL * 1ULL;
class DataFrame : public XuanWu::DataFrameBase
{
public:
    DataFrame() : XuanWu::DataFrameBase()
    {
        data = new char[buff_size];
        memset(data, 0xFF, sizeof(char) * buff_size);
    }

    ~DataFrame()
    {
        delete[] data;
    }

    virtual void* frameBodyData() const override
    {
        return data;
    }

    virtual uint64_t dataSize() const override
    {
        return buff_size;
    }

    virtual bool deserialize(char* data, uint64_t size) override
    {
        if (size != buff_size) {
            return false;
        }

        memcpy(this->data, data, buff_size * sizeof(char));
        return true;
    }

    bool check()
    {
        char c = 0xFF;
        for (int i = 0; i < buff_size; ++i) {
            if (data[i] != c) {
                return false;
            }
        }
        return true;
    }

    void zero()
    {
        memset(data, 0x00, buff_size * sizeof(char));
    }

private:
    char* data;
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
        XuanWu::FileWriter writer(metaHead);
        writer.createNewFile("D:/tmp/test.dat");
        XuanWu::DataFrameBase* df = new DataFrame();
        for (uint64_t i = 0; i < size / buff_size; ++i) {
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
        for (int i = 0; i < segmentItemIndex.size(); ++i) {
            DataFrame d;
            d.zero();
            reader.getDataFrame(i, &d);
            std::cout << i << "\t"
                      << segmentItemIndex[i]._DataPosition << "\t"
                      << segmentItemIndex[i]._DataLength << "\t"
                      << "Data Check:\t" << d.check()
                      << std::endl;
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