add_subdirectory(settings)

set(SRC
	transform.cpp

	transform.h
)

add_library(view-dialogs ${SRC})
add_dependencies(view-dialogs uic-dialogs)

