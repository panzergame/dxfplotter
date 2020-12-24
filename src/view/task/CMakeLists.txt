set(SRC
	path.cpp
	task.cpp
	tasklistmodel.cpp

	path.h
	task.h
	tasklistmodel.h
)

add_library(view-task ${SRC})
add_dependencies(view-task generate_config uic)
