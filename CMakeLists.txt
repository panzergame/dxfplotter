cmake_minimum_required(VERSION 3.14.0)

project(dxfplotter)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/template)

find_package(Qt5 COMPONENTS REQUIRED 
	Widgets
	Gui
)

find_package(nanoflann)

# Needed for qt
add_definitions(-fPIC)

include_directories(
	src
	thirdparty
	template
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	nanoflann::nanoflann
)

add_subdirectory(template)
add_subdirectory(thirdparty)
add_subdirectory(src)

add_executable(dxfplotter main.cpp)
target_link_libraries(dxfplotter
	Qt5::Widgets
	Qt5::Gui
	view
	control
	geometry
	model
	dxf
	converter
	spline
	libdxfrw
)
