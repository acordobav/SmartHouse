cmake_minimum_required(VERSION 3.0.0)
project(sm-server VERSION 0.1.0)

include(CTest)
enable_testing()

# Find the Pistache library.
find_package(Pistache 0.0.2 REQUIRED)

add_executable(sm-server main.cpp)
target_link_libraries(${PROJECT_NAME} pistache_shared)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
