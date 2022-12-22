set(SRC
	toolpath.cpp
	viewport.cpp

	toolpath.h
	viewport.h
)

add_library(view-view3d-internal ${SRC})
target_link_libraries(view-view3d-internal PRIVATE ${VTK_LIBRARIES})
