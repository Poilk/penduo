project(logging)

add_library(${PROJECT_NAME} Logger.cpp easyloggingpp/easylogging++.cc)

target_link_libraries(${PROJECT_NAME})