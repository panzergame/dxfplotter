set(SRC
	application.cpp
	layer.cpp
	path.cpp
	pathsettings.cpp
	pathgroupsettings.cpp
	renderable.cpp
	task.cpp

	application.h
	layer.h
	path.h
	pathsettings.h
	pathgroupsettings.h
	renderable.h
	task.h
	taskmodelobserver.h
)

add_library(model ${SRC})
add_dependencies(model generate_config)
add_coverage(model)
