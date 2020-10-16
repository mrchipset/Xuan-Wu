#ifndef SHARE_EXCEPTION_COMMON_H
#define SHARE_EXCEPTION_COMMON_H

#include "api.h"
#include <exception>
#include <stdexcept>
#include <string>

namespace XuanWu
{

class XUANWU_API CommonException : public std::exception
{
public:
    explicit CommonException(char const* const _Message) noexcept;
    explicit CommonException(std::string const& _Message) noexcept;
};

class XUANWU_API RuntimeException : public CommonException
{
public:
    explicit RuntimeException(std::string const& _Message) noexcept;
};

}   //end namespace XuanWu

#endif

