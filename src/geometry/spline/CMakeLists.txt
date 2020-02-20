set(SRC
	bezier.cpp
	beziertobiarc.cpp
	cubicsplinetobezier.cpp
# 	quadraticsplinetobezier.cpp
	splinetobezier.cpp

	bezier.h
	beziertobiarc.h
	cubicsplinetobezier.h
	quadraticsplinetobezier.h
	splinetobezier.h
)

add_library(spline ${SRC})

