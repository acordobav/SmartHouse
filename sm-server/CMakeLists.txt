cmake_minimum_required(VERSION 3.0.0)
project(sm-server VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lwiringPi")

# Find the Pistache library.
find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

# Find the RapidJSON library.
find_package(RapidJSON REQUIRED PATHS /opt/rapidjson)
include_directories(${RAPIDJSON_INCLUDE_DIRS})

# Find the SmartHouse library
find_package(smarthouse REQUIRED PATHS /opt/smarthouse)
INCLUDE_DIRECTORIES(/opt/smarthouse/include)
LINK_DIRECTORIES(/opt/smarthouse/lib)

# Find the Boost library
find_package(Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(sm-server main.cpp)
target_link_libraries(${PROJECT_NAME} smarthouse) 
target_link_libraries(${PROJECT_NAME} PkgConfig::Pistache)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
