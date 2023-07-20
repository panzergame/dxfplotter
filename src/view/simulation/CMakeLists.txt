add_subdirectory(internal)

set(SRC
	simulation.cpp

	simulation.h
)

add_library(view-simulation ${SRC})
add_dependencies(view-simulation generate-config uic)
