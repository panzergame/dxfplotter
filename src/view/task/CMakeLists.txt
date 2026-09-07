set(SRC
	path.cpp
	task.cpp
	pathlistmodel.cpp
	layertreemodel.cpp
)

add_module_library(view-task ${SRC})
target_link_libraries(view-task PUBLIC model model-application PRIVATE config geometry)
add_dependencies(view-task uic)

