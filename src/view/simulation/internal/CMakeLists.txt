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
