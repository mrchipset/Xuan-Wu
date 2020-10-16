#ifndef SHARE_UTILITY_H
#define SHARE_UTILITY_H

#include "api.h"

#define XUANWU_MACRO_STRING(param) #param
#define XUANWU_LINE_TRACE(msg) XUANWU_MACRO_STRING((__FILE__)##("::(")##(__LINE__)##")\t"##(msg))
#define XUANWU_FUNC_TRACE(msg) XUANWU_MACRO_STRING((__FUNCTION__)##(":\t")##(msg))
#define XUANWU_TRACE_ALL(msg) XUANWU_MACRO_STRING((__FILE__)##("::(")##(__LINE__)##")"##(__FUNCTION__)##(":\t")##(msg))


namespace XuanWu
{

}   //end namespace XuanWu
#endif