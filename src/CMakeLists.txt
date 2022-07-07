#find_package(Boost REQUIRED)

file(GLOB penduo_net_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} net/*.cpp)
file(GLOB penduo_base_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} base/*.cpp)
cmake_print_variables(penduo_net_srcs)
cmake_print_variables(penduo_base_srcs)

add_library(penduo STATIC
        ${penduo_net_srcs}
        ${penduo_base_srcs}
        base/timerfd/Timerfd.cpp
        base/timerfd/TimerfdCreator.cpp
        base/timerfd/$<IF:$<BOOL:${PENDUO_USE_NAIVE_TIMERFD}>,TimerfdNaive.cpp,TimerfdThreadSimulation.cpp>
        )
if(PENDUO_USE_NAIVE_TIMERFD)
    add_definitions(
            -DPENDUO_USE_NAIVE_TIMERFD
    )
endif()

target_include_directories(penduo
        PRIVATE ${Boost_INCLUDE_DIRS}
        PUBLIC .
        )

target_link_libraries(penduo logging)

add_subdirectory(net/tests)
add_subdirectory(base/tests)
add_subdirectory(base/logging)
