add_subdirectory(filter)

set(SRC
	arc.cpp
	bezier.cpp
	biarc.cpp
	bulge.cpp
	circle.cpp
	cubicspline.cpp
	pocketer.cpp
	polyline.cpp
	quadraticspline.cpp
	rect.cpp
	spline.cpp

	arc.h
	bezier.h
	biarc.h
	bulge.h
	circle.h
	cubicspline.h
	polyline.h
	quadraticspline.h
	spline.h
	rect.h
	utils.h
)

add_library(geometry ${SRC})
add_coverage(geometry)
