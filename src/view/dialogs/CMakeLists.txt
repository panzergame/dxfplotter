add_subdirectory(settings)

set(SRC
	mirror.cpp
	transform.cpp
	setorigin.cpp
)

add_module_library(view-dialogs ${SRC})
target_link_libraries(view-dialogs PUBLIC geometry model model-application)
add_dependencies(view-dialogs uic-dialogs)

