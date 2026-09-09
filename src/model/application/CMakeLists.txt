# Application and Simulation orchestrate the importers/exporters, which in turn consume
# model types. Keeping them in their own target keeps the CMake target graph acyclic.
set(SRC
	simulation.cpp
	application.cpp
	documentmodelobserver.cpp
)

add_module_library(model-application ${SRC})
target_link_libraries(model-application
	PUBLIC model
	PRIVATE
		config
		common
		geometry
		geometry-filter
		importer-dxf
		importer-dxfplot
		exporter-gcode
		exporter-dxfplot
		exporter-renderer
)
add_coverage(model-application)
