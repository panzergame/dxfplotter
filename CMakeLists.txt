cmake_minimum_required(VERSION 3.15)

project(dxfplotter)

include(CTest)
include(CPack)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tristan Porteries")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_TESTS OFF)
set(BUILD_SAMPLES OFF)
set(BUILD_EXAMPLES OFF)
set(USE_SCIP OFF)
set(USE_COINOR OFF)
set(BUILD_FLATZINC OFF)
set(USE_HIGHS OFF)
set(JUST_INSTALL_CEREAL ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Compilation flag with all warnings.
if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-fPIC -Wall -Wextra)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEMPLATE_DIR ${PROJECT_SOURCE_DIR}/template)

find_package(codecov)

find_package(PythonInterp REQUIRED)

find_package(Qt5 COMPONENTS REQUIRED
	Core
	Widgets
	Gui
	Svg
	3DCore
	3DExtras
)

#qt_standard_project_setup()

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/cereal/include
	thirdparty/cavaliercontours/include
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	thirdparty/units/include
	thirdparty/yaml-cpp/include
	thirdparty/or-tools
	template
	${CMAKE_BINARY_DIR}/src
	${CMAKE_BINARY_DIR}/template
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt53DCore_INCLUDE_DIRS}
	${Qt53DExtras_INCLUDE_DIRS}
)

set(LINK_LIBRARIES
	resource
	view
	view-dialogs
	view-dialogs-settings
	view-task
	view-view2d
	view-simulation
	view-simulation-internal
	model
	config
	importer-dxf
	importer-dxfplot
	exporter-gcode
	exporter-dxfplot
	exporter-renderer
	geometry
	geometry-filter
	libdxfrw
	fmt::fmt
	Qt5::Widgets
	Qt5::Svg
	Qt5::3DCore
	Qt5::3DExtras
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
