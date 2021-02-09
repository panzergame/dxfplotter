set(SRC
	polyline.cpp
	verticalspeed.cpp
)

add_executable(gtest ${SRC} main.cpp)
target_include_directories(gtest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(gtest ${GTEST_LIBRARIES} ${LINK_LIBRARIES} pthread)

add_test(gtest gtest)
