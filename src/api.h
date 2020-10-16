#ifndef XUANWU_API_H
#define XUANWU_API_H



#ifdef SHARE_LIB
#ifdef SHARE_LIB_EXPORT
#define XUANWU_API __declspec( dllexport )
#else
#define XUANWU_API __declspec( dllimport )
#endif
#else
#define XUANWU_API
#endif


#endif