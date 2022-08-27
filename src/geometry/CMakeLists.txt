set(SRC
	arc.cpp
	assembler.cpp
	bezier.cpp
	biarc.cpp
	bulge.cpp
	cleaner.cpp
	circle.cpp
	cubicspline.cpp
	pocketer.cpp
	polyline.cpp
	quadraticspline.cpp
	rect.cpp
	sorter.cpp
	spline.cpp

	arc.h
	assembler.h
	bezier.h
	biarc.h
	bulge.h
	cleaner.h
	circle.h
	cubicspline.h
	polyline.h
	quadraticspline.h
	sorter.h
	spline.h
	rect.h
	utils.h
)

add_library(geometry ${SRC})
add_coverage(geometry)
