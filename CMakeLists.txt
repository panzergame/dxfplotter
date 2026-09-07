cmake_minimum_required(VERSION 4.0)
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(CMAKE_CXX_MODULE_DYNDEP ON)

# Enable module scanning in version 3.28 and above
set(CMAKE_CXX_SCAN_FOR_MODULES ${CMAKE_CXX_MODULE_DYNDEP})

# Enable module scanning in version 3.26 and 3.27 using extension key
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ${CMAKE_CXX_MODULE_DYNDEP})
if (CMAKE_MAJOR_VERSION STREQUAL "3" AND CMAKE_MINOR_VERSION STREQUAL "26")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
elseif (CMAKE_MAJOR_VERSION STREQUAL "3" AND CMAKE_MINOR_VERSION STREQUAL "27")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
endif()

# Allow cmake std module for stdlibc++ or msvc
if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.4.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
elseif (CMAKE_VERSION VERSION_GREATER_EQUAL "4.3.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
elseif (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0.3")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
elseif (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
endif()

project(dxfplotter)

option(WITH_ORTOOLS OFF "Enable ortools support for path optimization")

include(CTest)
include(CPack)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tristan Porteries")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(BUILD_TESTS OFF)
set(BUILD_SAMPLES OFF)
set(BUILD_EXAMPLES OFF)
set(USE_SCIP OFF)
set(USE_COINOR OFF)
set(BUILD_FLATZINC OFF)
set(USE_HIGHS OFF)
set(JUST_INSTALL_CEREAL ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Compilation flag with all warnings.
if(MSVC)
	add_compile_options(/W4 /Zc:__cplusplus)
else()
	add_compile_options(-fPIC -Wall -Wextra -DNDEBUG)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEMPLATE_DIR ${PROJECT_SOURCE_DIR}/template)

find_package(codecov)

find_package(Python 3.10 REQUIRED)
if (WITH_ORTOOLS)
	find_package(ortools CONFIG REQUIRED)
endif()

find_package(Qt6 COMPONENTS REQUIRED
	Core
	Widgets
	Gui
	Svg
	3DCore
	3DExtras
)

qt_standard_project_setup()

add_definitions(${Qt6Core_DEFINITIONS})

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/cereal/include
	thirdparty/cavaliercontours/include
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	thirdparty/units/include
	thirdparty/or-tools
	template
	${CMAKE_BINARY_DIR}/src
	${CMAKE_BINARY_DIR}/template
	${Qt6Widgets_INCLUDE_DIRS}
	${Qt6Gui_INCLUDE_DIRS}
	${Qt63DCore_INCLUDE_DIRS}
	${Qt63DExtras_INCLUDE_DIRS}
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
	common
	libdxfrw
	fmt::fmt
	Qt6::Widgets
	Qt6::Svg
	Qt6::3DCore
	Qt6::3DExtras
)

include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
# Third party sources are plain headers and sources, scanning them for modules only costs build time.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
add_subdirectory(thirdparty EXCLUDE_FROM_ALL)
set(CMAKE_CXX_SCAN_FOR_MODULES ${CMAKE_CXX_MODULE_DYNDEP})

include(cmake/modules.cmake)


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
