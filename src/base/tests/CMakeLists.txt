
macro(generate_base_test file_name)
    set(test_name unittest_base_${file_name})
    add_executable(${test_name} ${file_name})
    target_link_libraries(${test_name} penduo pthread)
endmacro()


file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
foreach (file ${files})
    generate_base_test(${file})
endforeach ()
