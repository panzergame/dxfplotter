include(FetchContent)

FetchContent_Declare(
  	googletest
  	GIT_REPOSITORY https://github.com/google/googletest.git
  	GIT_TAG release-1.10.0
)

if (${CMAKE_VERSION} VERSION_LESS "3.14")
	FetchContent_GetProperties(googletest)
	if(NOT googletest_POPULATED)
	  FetchContent_Populate(googletest)
	  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
	endif()
else()
	FetchContent_MakeAvailable(googletest)
endif()

set(SRC
	polyline.cpp
	verticalspeed.cpp
)

add_executable(dxfplotter-test ${SRC} main.cpp)
target_link_libraries(dxfplotter-test ${LINK_LIBRARIES} gtest_main)

enable_testing()
add_test(dxfplotter-test dxfplotter-test)
