set(SRC
	application.cpp
	config.cpp
	path.cpp
	pathsettings.cpp
	pathgroupsettings.cpp
	task.cpp

	application.h
	config.h
	configvariable.h
	path.h
	pathsettings.h
	pathgroupsettings.h
	task.h
)

add_library(model ${SRC})
