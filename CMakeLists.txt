cmake_minimum_required(VERSION 3.12.0)

project(dxfplotter)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/template)

set(BUILD_TESTS OFF)
set(BUILD_EXAMPLES OFF)

find_package(Qt5 COMPONENTS REQUIRED 
	Widgets
	Gui
)

add_compile_options(-fPIC -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unknown-pragmas)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
	add_compile_options(-Wno-error=deprecated-copy)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	template
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
)

set(LINK_LIBRARIES
	Qt5::Widgets
	Qt5::Gui
	view
	model
	config
	dxf
	gcode
	converter
	geometry
	libdxfrw
	fmt
)
include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(test) # TODO seulement en cas de test


add_executable(dxfplotter main.cpp)
target_link_libraries(dxfplotter ${LINK_LIBRARIES})
