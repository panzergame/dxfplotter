set(SRC
	list.cpp
	settings.cpp
	treemodel.cpp

	entry.h
	group.h
	list.h
	settings.h
	treemodel.h
)

add_library(view-settings ${SRC})
add_dependencies(view-settings generate_config uic-settings)
