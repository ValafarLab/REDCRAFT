cmake_minimum_required(VERSION 3.8)

set(Name RedcraftTests)
set(Sources redcraft/redcraft_tests.cpp)

add_executable(${Name} ${Sources})
target_link_libraries(${Name} PUBLIC gtest_main valpack redcat rude_config levmar)
target_include_directories(${Name} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(${Name} PRIVATE TEST_PATH="${PROJECT_SOURCE_DIR}/test/redcraft/")

add_test(
	NAME ${Name}
	COMMAND ${Name}
)


