project(logging)

find_package(glog REQUIRED)

add_library(${PROJECT_NAME} Logger.cpp)

target_link_libraries(${PROJECT_NAME} glog::glog)