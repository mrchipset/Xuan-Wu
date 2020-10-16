#ifndef SHARE_EXCEPTION_RUNTIME_H
#define SHARE_EXCEPTION_RUNTIME_H

#include "api.h"
#include "CommonException.h"

namespace XuanWu
{

class XUANWU_API ConstructorException : public RuntimeException
{
public:
    explicit ConstructorException(std::string const& _Message);
};

class XUANWU_API NotImplementedException : public RuntimeException
{
public:
    explicit NotImplementedException(std::string const& _Message);
};

}   //end namespace XuanWu

#endif