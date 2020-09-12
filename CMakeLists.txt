cmake_minimum_required(VERSION 3.9)

project(dxfplotter)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TEMPLATE_DIR ${PROJECT_SOURCE_DIR}/template)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${TEMPLATE_DIR}/uic)

# Append custom package finder modules (e.g tinyxml2)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Qt5 COMPONENTS REQUIRED 
	Widgets
	Gui
)

find_package(TinyXML2 REQUIRED)
find_package(yaml-cpp REQUIRED)

# Compilation flag with all warning and error as much as possible on warning.
add_compile_options(-fPIC -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-expansion-to-defined -Wno-error=missing-field-initializers)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
	add_compile_options(-Wno-error=deprecated-copy)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRS
	src
	thirdparty
	thirdparty/cavaliercontours/include
	thirdparty/fmt/include
	thirdparty/nanoflann/include
	thirdparty/tomlplusplus/include
	template
	${CMAKE_BINARY_DIR}/src
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${TinyXML2_INCLUDE_DIRS}
	${YAML_CPP_INCLUDE_DIRS}
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
	${TinyXML2_LIBRARIES}
	${YAML_CPP_LIBRARIES}
)

include_directories(${INCLUDE_DIRS})

add_subdirectory(template)
add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(test) # TODO seulement en cas de test

add_executable(dxfplotter src/main.cpp)
target_link_libraries(dxfplotter ${LINK_LIBRARIES})

install(TARGETS dxfplotter DESTINATION bin)
