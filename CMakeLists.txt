cmake_minimum_required(VERSION 3.0.0)
project(xuanwu VERSION 0.1.0)

include(CTest)
enable_testing()
set(USE_SHARED ON CACHE INTERNAL "library link option")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)

set(C_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED ON)
set(C_STANDARD 11)
set(CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)


# message(${PROJECT_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(test)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
