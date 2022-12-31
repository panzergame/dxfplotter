add_subdirectory(internal)

set(SRC
	simulation.cpp

	simulation.h
)

add_library(view-simulation ${SRC})
target_link_libraries(view-simulation PRIVATE ${VTK_LIBRARIES})
add_dependencies(view-simulation generate-config uic)
