#include <exporter.h>
#include <iostream> // TODO

namespace Exporter::GCode
{

class CommandStream
{
private:
	std::stringstream &m_stream;
	bool m_addSpace;

public:
	explicit CommandStream(std::stringstream &stream)
		:m_stream(stream),
		m_addSpace(false)
	{
	}

	~CommandStream()
	{
		m_stream << "\n";
	}

	template <class T>
    inline CommandStream &operator<<(T data)
	{
		m_stream << data;
		if (m_addSpace) {
			m_stream << " ";
		}
		m_addSpace = true;
		return *this;
	}
};

class PathConverter
{
private:
	const Model::Path::Settings &m_settings;
	std::stringstream &m_stream;

public:
	explicit PathConverter(const Model::Path::Settings &settings, std::stringstream &stream)
		:m_settings(settings),
		m_stream(stream)
	{
		CommandStream(m_stream) << "M1";
	}

	~PathConverter()
	{
		CommandStream(m_stream) << "M2";
	}

	void linearMove(const QVector2D &to)
	{
		
	}

	void fastMove(const QVector2D &to)
	{
		CommandStream(m_stream) << "X" << to.x() << "Y" << to.y();
	}

	void cwArc();
	void ccwArc();
};

void Exporter::convertToGCode(const Model::Task &task)
{
	task.forEachPath([this](Model::Path *path){ convertToGCode(path); });
}

void Exporter::convertToGCode(Model::Path *path)
{
	PathConverter converter(path->settings(), m_fileContent);
	convertToGCode(converter, path->polyline());
}

void Exporter::convertToGCode(PathConverter &converter, const Geometry::Polyline &polyline)
{
	converter.fastMove(polyline.start());

	polyline.forEachBulge([this, &converter](const Geometry::Bulge &bulge){ convertToGCode(converter, bulge); });
}

void Exporter::convertToGCode(PathConverter &converter, const Geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		converter.linearMove(bulge.end());
	}
	else {
		
	}
}


Exporter::Exporter(const Model::Task &task, const std::string &filename)
{
	convertToGCode(task);
	std::cout << m_fileContent.str();
}

}

