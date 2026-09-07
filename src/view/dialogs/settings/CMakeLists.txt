set(SRC
	list.cpp
	settings.cpp
	treemodel.cpp
	entry.cpp
	group.cpp
)

add_module_library(view-dialogs-settings ${SRC})
target_link_libraries(view-dialogs-settings PUBLIC common model model-application PRIVATE config geometry)
add_dependencies(view-dialogs-settings uic-dialogs-settings)
