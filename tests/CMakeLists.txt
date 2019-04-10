CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project(libq-tests) 

set(tests
    as_native_cases
    precision
    range
)

add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp)
set_target_properties(example PROPERTIES FOLDER "tests")
target_link_libraries(example PUBLIC libq)

foreach(name ${tests})
    add_executable(${name} ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
    set_target_properties(${name} PROPERTIES FOLDER "tests")
    target_link_libraries(${name} PUBLIC libq Boost::unit_test_framework)
    add_test(NAME test_${name} COMMAND $<TARGET_FILE:${name}>)
endforeach()

target_link_libraries(precision PUBLIC Boost::log Boost::log_setup)
target_compile_definitions(precision PRIVATE $<$<BOOL:Boost::dynamic_linking>:BOOST_ALL_DYN_LINK>)
