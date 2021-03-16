cmake_minimum_required(VERSION 3.0.0)
project(sm-server VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find the Pistache library.
find_package(Pistache 0.0.2 REQUIRED)

# Find the Websocketpp library
find_package(websocketpp REQUIRED PATHS /opt/websocketpp)

# Find the Boost library
find_package(Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(sm-server main.cpp)
target_link_libraries(${PROJECT_NAME} pistache_shared)
target_link_libraries(${PROJECT_NAME} websocketpp::websocketpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
