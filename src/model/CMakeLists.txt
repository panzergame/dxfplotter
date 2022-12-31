set(SRC
	application.cpp
	document.cpp
	layer.cpp
	offsettedpath.cpp
	path.cpp
	pathsettings.cpp
	pathgroupsettings.cpp
	renderable.cpp
	simulation.cpp
	task.cpp

	application.h
	document.h
	layer.h
	path.h
	offsettedpath.h
	pathsettings.h
	pathgroupsettings.h
	renderable.h
	simulation.h
	task.h
	documentmodelobserver.h
)

add_library(model ${SRC})
add_dependencies(model generate-config)
add_coverage(model)
