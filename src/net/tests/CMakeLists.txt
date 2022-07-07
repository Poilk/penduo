
macro(generate_net_test folder file_name)
    set(test_name unittest_net_${file_name})
    add_executable(${test_name} ${folder}/${file_name})
    target_link_libraries(${test_name} penduo pthread)
endmacro()

foreach (folder eventloop tcp)
    file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${folder} ${folder}/*.cpp)

    foreach (file ${files})
        #message(STATUS "folder ${CMAKE_CURRENT_SOURCE_DIR}/eventloo, file: ${file}")
        generate_net_test(${folder} ${file})
    endforeach ()

endforeach ()

