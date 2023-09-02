include(FetchContent)

FetchContent_Declare(
  	googletest
  	GIT_REPOSITORY https://github.com/google/googletest.git
  	GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

find_package(Qt5 COMPONENTS REQUIRED
	Test
)

set(SRC
	arc.cpp
	assembler.cpp
	bulge.cpp
	dxfplotexporter.cpp
	dxfplotimporter.cpp
	exporterfixture.cpp
	exporterrenderer.cpp
	gcodeexporter.cpp
	path.cpp
	pathsettings.cpp
	pocketer.cpp
	polyline.cpp
	polylineutils.cpp
	serializer.cpp
	simulation.cpp
	task.cpp
	verticalspeed.cpp

	exporterfixture.h
	polylineutils.h
)

add_executable(dxfplotter-test ${SRC} main.cpp)

target_include_directories(dxfplotter-test PRIVATE ${Qt5Test_INCLUDE_DIRS})
target_link_libraries(dxfplotter-test ${LINK_LIBRARIES} Qt5::Test gtest_main)

add_coverage(dxfplotter-test)

enable_testing()
gtest_add_tests(TARGET dxfplotter-test)
