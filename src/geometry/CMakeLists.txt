add_subdirectory(filter)

set(SRC
	utils.cpp
	circle.cpp
	arc.cpp
	line.cpp
	rect.cpp
	bulge.cpp
	polyline.cpp
	biarc.cpp
	bezier.cpp
	spline.cpp
	cubicspline.cpp
	quadraticspline.cpp
	pocketer.cpp
)

if (WITH_ORTOOLS)
	list(APPEND SRC
		orderoptimizer.cpp
	)
endif()

add_module_library(geometry ${SRC} NON_MODULE_SOURCES cavcutils.h)
target_link_libraries(geometry PUBLIC common)
if (WITH_ORTOOLS)
	target_link_libraries(geometry PUBLIC ortools::ortools)
	target_compile_definitions(geometry PUBLIC WITH_ORTOOLS)
endif()

add_coverage(geometry)
