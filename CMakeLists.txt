cmake_minimum_required(VERSION 3.14)

project(dxfplotter)

include(CTest)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_TESTS OFF)
set(JUST_INSTALL_CEREAL ON)

# Compilation flag with all warnings.
add_compile_options(-fPIC -Wall -Wextra)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
	add_compile_options(-Wno-error=deprecated-copy -Wno-error=maybe-uninitialized)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEMPLATE_DIR ${PROJECT_SOURCE_DIR}/template)

find_package(codecov)

find_package(Qt5 COMPONENTS REQUIRED 
	Widgets
	Gui
)

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/cereal/include
	thirdparty/cavaliercontours/include
	thirdparty/eigen
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	thirdparty/yaml-cpp/include
	template
	${CMAKE_BINARY_DIR}/src
	${CMAKE_BINARY_DIR}/template
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
)

set(LINK_LIBRARIES
	resource
	view
	view-dialogs
	view-dialogs-settings
	view-task
	view-view2d
	model
	config
	importer-dxf
	importer-dxfplot
	exporter-gcode
	exporter-dxfplot
	geometry
	libdxfrw
	fmt::fmt
	Qt5::Widgets
	yaml-cpp
)

include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
add_subdirectory(thirdparty EXCLUDE_FROM_ALL)
add_subdirectory(resource)
add_subdirectory(src)

if (BUILD_TESTING)
	add_subdirectory(test)
endif()

add_executable(dxfplotter src/main.cpp)
target_link_libraries(dxfplotter ${LINK_LIBRARIES})
add_coverage(dxfplotter)

install(TARGETS dxfplotter DESTINATION bin)

coverage_evaluate()
