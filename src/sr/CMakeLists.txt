cmake_minimum_required(VERSION 3.0.0)
project(libxuanwu_share VERSION 0.1.0)

set(C_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED ON)
set(C_STANDARD 11)
set(CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../)


aux_source_directory(. DIR_SRCS)

FILE(GLOB EXCEPTION_SRCS "Exception/*.cc" "Exception/*.h")

set(LIB_XUANWU_SHARE_SRCS
    ${DIR_SRCS}   
    ${EXCEPTION_SRCS}
    CACHE INTERNAL
    "lib_xuanwu_share_src"
)

if (USE_SHARED)
add_definitions(-DSHARE_LIB)
add_definitions(-DSHARE_LIB_EXPORT)
set(LIB_TYPE SHARED)
else()
set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE} ${LIB_XUANWU_SHARE_SRCS})
