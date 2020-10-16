#ifndef IO_PROCESS_H
#define IO_PROCESS_H

#include "api.h"
#include <cstdint>
#include <string>
#include <Windows.h>

namespace XuanWu
{
/**
 * @brief The Process Data Type
 *
 */
struct ProcessData {
    uint32_t pid;   ///< process id
    bool inherited; ///< inherited the parent process
    std::string name; ///< process name
    uint32_t desiredAccess; ///< desired access privilage
    HANDLE handle;  ///< windows native handle
};

class XUANWU_API Process
{
public:
    Process(uint32_t pid, bool inherited, uint32_t desiredAccess);
    Process(const ProcessData& handle);
    virtual ~Process();

    bool writeData(uint64_t addr, uint8_t* buffer, uint64_t size);
    bool readData(uint64_t addr, uint8_t* buffer, uint64_t len);
private:
protected:
    ProcessData m_ProcessData;
};

}   //end namespace XuanWu


#endif