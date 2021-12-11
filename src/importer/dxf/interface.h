#pragma once

#include <libdxfrw/drw_interface.h>

namespace importer::dxf
{

class Importer;

class Interface : public DRW_Interface
{
private:
	Importer &m_importer;

	virtual void addHeader(const DRW_Header* data) override;
	virtual void addLType(const DRW_LType& data) override;
	virtual void addLayer(const DRW_Layer& data) override;
	virtual void addDimStyle(const DRW_Dimstyle& data) override;
	virtual void addVport(const DRW_Vport& data) override;
	virtual void addTextStyle(const DRW_Textstyle& data) override;
	virtual void addAppId(const DRW_AppId& data) override;
	virtual void addBlock(const DRW_Block& data) override;
	virtual void setBlock(const int handle) override;
	virtual void endBlock() override;
	virtual void addPoint(const DRW_Point& data) override;
	virtual void addLine(const DRW_Line& data) override;
	virtual void addRay(const DRW_Ray& data) override;
	virtual void addXline(const DRW_Xline& data) override;
	virtual void addArc(const DRW_Arc& data) override;
	virtual void addCircle(const DRW_Circle& data) override;
	virtual void addEllipse(const DRW_Ellipse& data) override;
	virtual void addLWPolyline(const DRW_LWPolyline& data) override;
	virtual void addPolyline(const DRW_Polyline& data) override;
	virtual void addSpline(const DRW_Spline* data) override;
	virtual void addKnot(const DRW_Entity& data) override;
	virtual void addInsert(const DRW_Insert& data) override;
	virtual void addTrace(const DRW_Trace& data) override;
	virtual void add3dFace(const DRW_3Dface& data) override;
	virtual void addSolid(const DRW_Solid& data) override;
	virtual void addMText(const DRW_MText& data) override;
	virtual void addText(const DRW_Text& data) override;
	virtual void addDimAlign(const DRW_DimAligned *data) override;
	virtual void addDimLinear(const DRW_DimLinear *data) override;
	virtual void addDimRadial(const DRW_DimRadial *data) override;
	virtual void addDimDiametric(const DRW_DimDiametric *data) override;
	virtual void addDimAngular(const DRW_DimAngular *data) override;
	virtual void addDimAngular3P(const DRW_DimAngular3p *data) override;
	virtual void addDimOrdinate(const DRW_DimOrdinate *data) override;
	virtual void addLeader(const DRW_Leader *data) override;
	virtual void addHatch(const DRW_Hatch *data) override;	
	virtual void addViewport(const DRW_Viewport& data) override;
	virtual void addImage(const DRW_Image *data) override;
	virtual void linkImage(const DRW_ImageDef *data) override;
	virtual void addComment(const char* comment) override;
    virtual void addPlotSettings(const DRW_PlotSettings *data) override;

	virtual void writeHeader(DRW_Header& data) override;
	virtual void writeBlocks() override;
	virtual void writeBlockRecords() override;
	virtual void writeEntities() override;
	virtual void writeLTypes() override;
	virtual void writeLayers() override;
	virtual void writeTextstyles() override;
	virtual void writeVports() override;
	virtual void writeDimstyles() override;
    virtual void writeObjects() override;
	virtual void writeAppId() override;

public:
	explicit Interface(Importer &importer);
};

}
