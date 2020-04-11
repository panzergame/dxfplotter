cmake_minimum_required(VERSION 3.9)

project(dxfplotter)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TEMPLATE_DIR ${PROJECT_SOURCE_DIR}/template)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${TEMPLATE_DIR}/uic)

find_package(Qt5 COMPONENTS REQUIRED 
	Widgets
	Gui
)

add_compile_options(-fPIC -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-expansion-to-defined -Wno-error=missing-field-initializers)

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
	${CMAKE_BINARY_DIR}/src
)

set(LINK_LIBRARIES
	view
	model
	config
	dxf
	gcode
	geometry
	libdxfrw
	fmt
	Qt5::Widgets
)
include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(test) # TODO seulement en cas de test


add_executable(dxfplotter main.cpp)
target_link_libraries(dxfplotter ${LINK_LIBRARIES})
