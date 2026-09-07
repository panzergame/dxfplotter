set(SRC
	aggregable.cpp
	copy.cpp
	enumerate.cpp
	exception.cpp
	function.cpp
)

add_module_library(common ${SRC})
add_coverage(common)
