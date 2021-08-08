cmake_minimum_required(VERSION 3.11)

project(dxfplotter)

include(CTest)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Compilation flag with all warning and error as much as possible on warning.
add_compile_options(-fPIC -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-expansion-to-defined -Wno-error=missing-field-initializers)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
	add_compile_options(-Wno-error=deprecated-copy)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEMPLATE_DIR ${PROJECT_SOURCE_DIR}/template)

find_package(codecov)

find_package(Qt5 COMPONENTS REQUIRED 
	Widgets
	Gui
)

find_package(yaml-cpp REQUIRED)
find_package(fmt REQUIRED)

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/cavaliercontours/include
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	thirdparty/tomlplusplus/include
	template
	${CMAKE_BINARY_DIR}/src
	${CMAKE_BINARY_DIR}/template
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${YAML_CPP_INCLUDE_DIRS}
)

set(LINK_LIBRARIES
	resource
	view
	view-settings
	view-task
	view-view2d
	model
	config
	dxf
	gcode
	geometry
	libdxfrw
	fmt::fmt
	Qt5::Widgets
	${YAML_CPP_LIBRARIES}
)

include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
add_subdirectory(thirdparty)
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
