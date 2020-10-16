#include "CommonException.h"


using namespace XuanWu;
using namespace std;

CommonException::CommonException(char const* const _Message) noexcept
    : exception(_Message)
{
}

CommonException::CommonException(std::string const& _Message) noexcept
    : CommonException(_Message.c_str())
{
}

RuntimeException::RuntimeException(std::string const& _Message) noexcept
    : CommonException(_Message)
{
}

