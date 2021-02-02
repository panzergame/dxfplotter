
set(SRC

)

add_executable(test ${SRC} main.cpp)
target_include_directories(test PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(test ${GTEST_LIBRARIES} pthread)
