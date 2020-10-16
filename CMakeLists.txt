cmake_minimum_required(VERSION 3.0.0)
project(xuanwu VERSION 0.1.0)

include(CTest)
enable_testing()
set(USE_SHARED ON CACHE INTERNAL "library link option")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)

# message(${PROJECT_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(test)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
