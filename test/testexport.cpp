#include <control/application.h>
#include <exporter/gcode/exporter.h>

int main(int argc, char *argv[])
{
	Control::Application app(argv[1]);

	Exporter::GCode::Exporter exporter(app.task(), "");
}
