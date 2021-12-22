add_subdirectory(settings)

set(SRC
	mirror.cpp
	transform.cpp

	mirror.h
	transform.h
)

add_library(view-dialogs ${SRC})
add_dependencies(view-dialogs uic-dialogs)

