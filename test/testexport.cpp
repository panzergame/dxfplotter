#include <control/application.h>

int main(int argc, char *argv[])
{
	Control::Application app(argv[1]);
	app.exportToGcode("");
}
