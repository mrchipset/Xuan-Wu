#include "io/Process/Process.h"
#include "io/MagicData/FileWriter.h"
#include <iostream>
#include <chrono>

constexpr uint64_t buff_size = 1024ULL * 1ULL;
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

private:
    char* data;
};

int main()
{
    auto time_start = std::chrono::system_clock::now();
    const uint64_t size = 1024ULL * 1024ULL;
    try {
        XuanWu::FileMetaHead metaHead;
        memset(&metaHead, 0x00, sizeof(XuanWu::FileMetaHead));
        metaHead._FileVersionId = 0x0000000000000001ULL;
        metaHead._SegmentBlockSize = 4096ULL;
        XuanWu::FileWriter writer(metaHead);
        writer.createNewFile("D:/tmp/test.dat");
        XuanWu::DataFrameBase* df = new DataFrame();
        for (uint64_t i = 0; i < size / buff_size; ++i) {
            writer.writeDataFrame(df);
        }
        delete df;
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