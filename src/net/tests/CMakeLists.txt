
macro(generate_net_test file_name)
    set(test_name unittest_net_${file_name})
    add_executable(${test_name} ${file_name})
    target_link_libraries(${test_name} penduo)
endmacro()


file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
foreach (file ${files})
    message(STATUS "folder ${CMAKE_CURRENT_SOURCE_DIR}, file: ${file}")
    generate_net_test(${file})
endforeach ()
