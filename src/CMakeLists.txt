cmake_minimum_required(VERSION 3.0.0)
project(libxuanwu VERSION 0.1.0)



# aux_source_directory(. DIR_SRCS)
# set(LIB_XUANWU_SRCS
#     ${DIR_SRCS}   
#     ${BD_SRCS} 
#     ${CR_SRCS} 
#     ${DP_SRCS} 
#     ${DS_SRCS} 
#     ${BD_SRCS} 
#     CACHE INTERNAL
#     "lib_xuanwu_src"
# )

if (USE_SHARED)
add_definitions(-DSHARE_LIB)
add_definitions(-DSHARE_LIB_EXPORT)
endif()

add_subdirectory(io)
add_subdirectory(sr)

# add_library(${PROJECT_NAME} ${LIB_XUANWU_SRCS})

