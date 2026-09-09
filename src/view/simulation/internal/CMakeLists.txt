set(SRC
	scene.cpp
	toolpath.cpp
	tool.cpp
	viewport.cpp
)

add_module_library(view-simulation-internal ${SRC})
target_link_libraries(view-simulation-internal Qt6::3DCore Qt6::Widgets Qt6::3DRender common geometry model model-application)