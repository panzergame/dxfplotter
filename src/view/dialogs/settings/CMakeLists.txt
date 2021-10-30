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

add_library(view-dialogs-settings ${SRC})
add_dependencies(view-dialogs-settings generate_config uic-dialogs-settings)
