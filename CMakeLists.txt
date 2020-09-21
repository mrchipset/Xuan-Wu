cmake_minimum_required(VERSION 3.0.0)
project(libxuanwu VERSION 0.1.0)

include(CTest)
enable_testing()
aux_source_directory(src SRC_DIR)
include_directories(include)



set(SRCS
    ${SRC_DIR}    
)

add_library(${PROJECT_NAME} ${SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
