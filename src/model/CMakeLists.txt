set(SRC
	application.cpp
	path.cpp
	pathsettings.cpp
	pathgroupsettings.cpp
	task.cpp

	application.h
	path.h
	pathsettings.h
	pathgroupsettings.h
	task.h
)

add_library(model ${SRC})
