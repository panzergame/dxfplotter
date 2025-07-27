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
target_link_libraries(view-dialogs-settings PRIVATE config)
add_dependencies(view-dialogs-settings uic-dialogs-settings)
