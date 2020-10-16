project(xuanwu_demo VERSION 0.1.0)
aux_source_directory(. DIR_SRCS)

include_directories(../../src)
if(USE_SHARED)
add_definitions(-DSHARE_LIB)
endif()

add_executable(${PROJECT_NAME} ${DIR_SRCS})
target_link_libraries(${PROJECT_NAME} libxuanwu_io)