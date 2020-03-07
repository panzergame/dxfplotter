set(SRC
	config.cpp
	path.cpp
	pathsettings.cpp
	pathgroupsettings.cpp
	task.cpp

	config.h
	configvariable.h
	path.h
	pathsettings.h
	pathgroupsettings.h
	task.h
	utils.h
)

add_library(model ${SRC})
