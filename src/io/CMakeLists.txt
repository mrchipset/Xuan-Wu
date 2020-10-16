cmake_minimum_required(VERSION 3.0.0)
project(libxuanwu_io VERSION 0.1.0)

set(C_STANDARD_REQUIRED 11)
set(CXX_STANDARD_REQUIRED 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../)


aux_source_directory(. DIR_SRCS)

FILE(GLOB PROCESS_SRCS "Process/*.cc" "Process/*.h")

set(LIB_XUANWU_IO_SRCS
    ${LIB_XUANWU_SRCS}
    ${DIR_SRCS}   
    ${PROCESS_SRCS}
    CACHE INTERNAL
    "lib_xuanwu_io_src"
)

if (USE_SHARED)
add_definitions(-DSHARE_LIB)
add_definitions(-DSHARE_LIB_EXPORT)
set(LIB_TYPE SHARED)
else()
set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE} ${LIB_XUANWU_IO_SRCS})
target_link_libraries(${PROJECT_NAME}
    libxuanwu_share
)
