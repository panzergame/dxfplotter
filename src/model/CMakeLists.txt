set(SRC
	application.cpp
	document.cpp
	documenthistory.cpp
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
	documenthistory.h
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
target_link_libraries(model PUBLIC geometry common PRIVATE config geometry-filter)
add_coverage(model)
