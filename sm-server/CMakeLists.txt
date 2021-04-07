cmake_minimum_required(VERSION 3.0.0)
project(sm-server VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lwiringPi")

# Find the Pistache library.
set(Pistache_FOUND 1)
find_package(Pistache 0.0.2 REQUIRED PATHS /opt/pistache)

# Find the RapidJSON library.
include_directories(/opt/rapidjson/include)
link_directories(/opt/rapidjson/lib)

# Find the SmartHouse library
INCLUDE_DIRECTORIES(/opt/smarthouse/include)
LINK_DIRECTORIES(/opt/smarthouse/lib)

# Find the Boost library
find_package(Boost 1.71.0 COMPONENTS program_options REQUIRED PATHS /opt/boost)
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(sm-server main.cpp)
target_link_libraries(${PROJECT_NAME} smarthouse) 
target_link_libraries(${PROJECT_NAME} pistache_shared)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
