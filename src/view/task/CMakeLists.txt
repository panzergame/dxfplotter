set(SRC
	path.cpp
	task.cpp
	pathlistmodel.cpp
	layertreemodel.cpp

	path.h
	task.h
	pathlistmodel.h
	layertreemodel.h
)

add_library(view-task ${SRC})
target_link_libraries(view-task PRIVATE config)
add_dependencies(view-task uic)
