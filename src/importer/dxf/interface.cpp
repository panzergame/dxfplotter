module;

#include <libdxfrw/drw_interface.h>

export module importer.dxf.interface;

import importer.dxf.importer;

namespace importer::dxf
{

#define PRINT_FUNC // std::cout << __func__ << std::endl;

}

export namespace importer::dxf
{

class Interface : public DRW_Interface
{
private:
	Importer& m_importer;

	void addHeader(const DRW_Header* data) override { PRINT_FUNC; }

	void addLType(const DRW_LType& data) override { PRINT_FUNC; }

	void addLayer(const DRW_Layer& data) override
	{
		m_importer.processEntity(data);
		PRINT_FUNC;
	}

	void addDimStyle(const DRW_Dimstyle& data) override { PRINT_FUNC; }

	void addVport(const DRW_Vport& data) override { PRINT_FUNC; }

	void addTextStyle(const DRW_Textstyle& data) override { PRINT_FUNC; }

	void addAppId(const DRW_AppId& data) override { PRINT_FUNC; }

	void addBlock(const DRW_Block& data) override
	{
		PRINT_FUNC;
		m_importer.startBlock();
	}

	void setBlock(const int handle) override { PRINT_FUNC; }

	void endBlock() override
	{
		PRINT_FUNC;
		m_importer.endBlock();
	}

	void addPoint(const DRW_Point& data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(data);
	}

	void addLine(const DRW_Line& data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(data);
	}

	void addRay(const DRW_Ray& data) override { PRINT_FUNC; }

	void addXline(const DRW_Xline& data) override { PRINT_FUNC; }

	void addArc(const DRW_Arc& data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(data);
	}

	void addCircle(const DRW_Circle& data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(data);
	}

	void addEllipse(const DRW_Ellipse& data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(data);
	}

	void addLWPolyline(const DRW_LWPolyline& data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(data);
	}

	void addPolyline(const DRW_Polyline& data) override { PRINT_FUNC; }

	void addSpline(const DRW_Spline* data) override
	{
		PRINT_FUNC;
		m_importer.processEntity(*data);
	}

	void addKnot(const DRW_Entity& data) override { PRINT_FUNC; }

	void addInsert(const DRW_Insert& data) override { PRINT_FUNC; }

	void addTrace(const DRW_Trace& data) override { PRINT_FUNC; }

	void add3dFace(const DRW_3Dface& data) override { PRINT_FUNC; }

	void addSolid(const DRW_Solid& data) override { PRINT_FUNC; }

	void addMText(const DRW_MText& data) override { PRINT_FUNC; }

	void addText(const DRW_Text& data) override { PRINT_FUNC; }

	void addDimAlign(const DRW_DimAligned* data) override { PRINT_FUNC; }

	void addDimLinear(const DRW_DimLinear* data) override { PRINT_FUNC; }

	void addDimRadial(const DRW_DimRadial* data) override { PRINT_FUNC; }

	void addDimDiametric(const DRW_DimDiametric* data) override { PRINT_FUNC; }

	void addDimAngular(const DRW_DimAngular* data) override { PRINT_FUNC; }

	void addDimAngular3P(const DRW_DimAngular3p* data) override { PRINT_FUNC; }

	void addDimOrdinate(const DRW_DimOrdinate* data) override { PRINT_FUNC; }

	void addLeader(const DRW_Leader* data) override { PRINT_FUNC; }

	void addHatch(const DRW_Hatch* data) override { PRINT_FUNC; }

	void addViewport(const DRW_Viewport& data) override { PRINT_FUNC; }

	void addImage(const DRW_Image* data) override { PRINT_FUNC; }

	void linkImage(const DRW_ImageDef* data) override { PRINT_FUNC; }

	void addComment(const char* comment) override { PRINT_FUNC; }

	void addPlotSettings(const DRW_PlotSettings* data) override { PRINT_FUNC; }

	void writeHeader(DRW_Header& data) override { PRINT_FUNC; }

	void writeBlocks() override { PRINT_FUNC; }

	void writeBlockRecords() override { PRINT_FUNC; }

	void writeEntities() override { PRINT_FUNC; }

	void writeLTypes() override { PRINT_FUNC; }

	void writeLayers() override { PRINT_FUNC; }

	void writeTextstyles() override { PRINT_FUNC; }

	void writeVports() override { PRINT_FUNC; }

	void writeDimstyles() override { PRINT_FUNC; }

	void writeObjects() override { PRINT_FUNC; }

	void writeAppId() override { PRINT_FUNC; }

public:
	explicit Interface(Importer& importer)
		: m_importer(importer)
	{
	}
};

}
