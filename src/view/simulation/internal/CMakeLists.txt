set(SRC
	toolpath.cpp
	viewport.cpp
	tool.cpp

	toolpath.h
	viewport.h
	tool.h
)

add_library(view-simulation-internal ${SRC})
target_link_libraries(view-simulation-internal PRIVATE ${VTK_LIBRARIES})
add_dependencies(view-simulation-internal generate-config uic)
