add_subdirectory(internal)

set(SRC
	viewport.cpp

	viewport.h
)

add_library(view-view3d ${SRC})
target_link_libraries(view-view3d PRIVATE ${VTK_LIBRARIES})
add_dependencies(view-view3d generate-config uic)
