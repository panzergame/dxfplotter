set(SRC
	qvector2d.cpp
	bulge.cpp
	polyline.cpp
	renderable.cpp
	pathsettings.cpp
	offsettedpath.cpp
	path.cpp
	layer.cpp
	task.cpp
)

add_module_library(serializer ${SRC})
target_link_libraries(serializer PUBLIC geometry model PRIVATE serializer_access)

add_module_library(serializer_access access.cpp)
