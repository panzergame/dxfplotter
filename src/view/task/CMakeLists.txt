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
add_dependencies(view-task generate_config uic)
