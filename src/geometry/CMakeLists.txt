set(SRC
	arc.cpp
	assembler.cpp
	bezier.cpp
	biarc.cpp
	bulge.cpp
	circle.cpp
	cubicspline.cpp
	polyline.cpp
# 	quadraticspline.cpp
	spline.cpp

	arc.h
	assembler.h
	bezier.h
	biarc.h
	bulge.h
	circle.h
	cubicspline.h
	polyline.h
	quadraticspline.h
	spline.h
	utils.h
)

add_library(geometry ${SRC})
