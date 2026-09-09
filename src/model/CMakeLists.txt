add_subdirectory(application)

set(SRC
	pathsettings.cpp
	renderable.cpp
	offsettedpath.cpp
	path.cpp
	layer.cpp
	task.cpp
	document.cpp
	documenthistory.cpp
	pathgroupsettings.cpp
)

add_module_library(model ${SRC})
target_link_libraries(model PUBLIC geometry common PRIVATE config geometry-filter)
add_coverage(model)
