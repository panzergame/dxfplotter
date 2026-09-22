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

# access.h stays a plain header: geometry's module interfaces friend-declare serializer::Access,
# so modularising it would make geometry and serializer circular CMake targets.
add_module_library(serializer ${SRC})
target_link_libraries(serializer PUBLIC geometry model)
