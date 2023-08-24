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

option(BUILD_WASM "Build to WebAssembly" OFF)

find_package(codecov)

find_package(PythonInterp REQUIRED)

if (BUILD_WASM)
	message(STATUS "Building to WebAssembly (Qt6_DIR ${Qt6_DIR} QT_HOST_PATH ${QT_HOST_PATH})")

	add_compile_definitions(BUILD_WASM)

	set(WITH_3D OFF)

	# set(QT_HOST_PATH "/home/tristan/Compilation/wasm-test/6.5.0/gcc_64/")
	set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
	list(APPEND CMAKE_PREFIX_PATH ${Qt6_DIR})
	# include(/home/tristan/Compilation/wasm-test/6.5.0/wasm_singlethread/lib/cmake/Qt6/QtPublicWasmToolchainHelpers.cmake)
else()
	set(WITH_3D ON)
endif()

find_package(Qt6 COMPONENTS REQUIRED
	Core
	Widgets
	Gui
	Svg
)

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/cereal/include
	thirdparty/cavaliercontours/include
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	thirdparty/units/include
	thirdparty/yaml-cpp/include
	template
	${CMAKE_BINARY_DIR}/src
	${CMAKE_BINARY_DIR}/template
	${Qt6Widgets_INCLUDE_DIRS}
	${Qt6Gui_INCLUDE_DIRS}
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
	exporter-renderer
	geometry
	geometry-filter
	libdxfrw
	fmt::fmt
	Qt::Core
	Qt::Gui
	Qt::Svg
	Qt::Widgets
	yaml-cpp
)

if (WITH_3D)
	add_compile_definitions(WITH_3D)

	find_package(Qt6 COMPONENTS REQUIRED
		3DCore
		3DExtras
	)

	list(APPEND INCLUDE_DIRS
		${Qt63DCore_INCLUDE_DIRS}
		${Qt63DExtras_INCLUDE_DIRS}
	)

	list(APPEND LINK_LIBRARIES
		view-simulation
		view-simulation-internal
		Qt::3DCore
		Qt::3DExtras
	)
endif()

include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
add_subdirectory(thirdparty EXCLUDE_FROM_ALL)
add_subdirectory(resource)
add_subdirectory(src)

if (BUILD_TESTING)
	add_subdirectory(test)
endif()

qt_add_executable(dxfplotter src/main.cpp)
target_link_libraries(dxfplotter PRIVATE ${LINK_LIBRARIES})

if (BUILD_WASM)
	target_link_options(dxfplotter PRIVATE -lidbfs.js)
endif()

add_coverage(dxfplotter)

install(TARGETS dxfplotter DESTINATION bin)

coverage_evaluate()
