#include "Process.h"
#include <iostream>
#include <sstream>

#include "sr/Exception/RuntimeException.h"
#include "sr/Utility.h"

using namespace XuanWu;
using namespace std;

Process::Process(uint32_t pid, bool inherited, uint32_t desiredAccess)
{
    m_ProcessData.pid = pid;
    m_ProcessData.desiredAccess = desiredAccess;
    m_ProcessData.handle = OpenProcess(desiredAccess, inherited, pid);
    if (m_ProcessData.handle == nullptr) {  // Open Error
        stringstream ss;
        ss << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ << ": "
           << "Open the Process Error";
        throw ConstructorException(ss.str());
    }

}

Process::Process(const ProcessData& data)
{
    m_ProcessData = data;
    m_ProcessData.handle = OpenProcess(m_ProcessData.desiredAccess,
                                       m_ProcessData.inherited,
                                       m_ProcessData.pid);
    if (m_ProcessData.handle == nullptr) {  // Open Error
        stringstream ss;
        ss << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ << ": "
           << "Open the Process Error";
        throw ConstructorException(ss.str());
    }
}

Process::~Process()
{
    if (m_ProcessData.handle != nullptr) {
        CloseHandle(m_ProcessData.handle);
        m_ProcessData.handle = nullptr;
    }
}

bool Process::writeData(uint64_t addr, uint8_t* buffer, uint64_t size)
{
    throw NotImplementedException(XUANWU_TRACE_ALL("Not Implmemented yet."));
    return false;
}

bool Process::readData(uint64_t addr, uint8_t* buffer, uint64_t len)
{
    throw NotImplementedException(XUANWU_TRACE_ALL("Not Implmemented yet."));
    return false;
}