set(SRC
	scene.cpp
	toolpath.cpp
	tool.cpp
	viewport.cpp

	scene.h
	toolpath.h
	tool.h
	viewport.h
)

add_library(view-simulation-internal ${SRC})
target_link_libraries(view-simulation-internal Qt6::3DCore Qt6::Widgets Qt6::3DRender)