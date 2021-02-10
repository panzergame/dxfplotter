#pragma once


namespace Importer::Dxf
{

class Layer
{
private:
	bool m_plot;

public:
	explicit Layer(bool plot);

	bool visible() const;
};

}

