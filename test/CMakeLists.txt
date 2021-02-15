include(FetchContent)

FetchContent_Declare(
  	googletest
  	GIT_REPOSITORY https://github.com/google/googletest.git
  	GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

set(SRC
	polyline.cpp
	verticalspeed.cpp
)

add_executable(dxfplotter-test ${SRC} main.cpp)
target_link_libraries(dxfplotter-test ${LINK_LIBRARIES} gtest_main)

enable_testing()
add_test(dxfplotter-test dxfplotter-test)
