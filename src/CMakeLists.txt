find_package(Boost REQUIRED)

add_library(penduo STATIC
        net/EventLoop.cpp
        )

target_include_directories(penduo
        PRIVATE ${Boost_INCLUDE_DIRS}
        PUBLIC base
        PUBLIC net
        )

target_link_libraries(penduo logging)

add_subdirectory(net/tests)
add_subdirectory(base/logging)
