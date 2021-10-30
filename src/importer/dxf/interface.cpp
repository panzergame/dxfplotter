#include <interface.h>
#include <importer.h>

#include <iostream>

namespace importer::dxf
{

Interface::Interface(Importer &importer)
	:m_importer(importer)
{
}

#define PRINT_FUNC //std::cout << __func__ << std::endl;

void Interface::addHeader(const DRW_Header* data)
{
	PRINT_FUNC;
}

void Interface::addLType(const DRW_LType& data)
{
	PRINT_FUNC;
}

void Interface::addLayer(const DRW_Layer& data)
{
	m_importer.processEntity(data);
	PRINT_FUNC;
}

void Interface::addDimStyle(const DRW_Dimstyle& data)
{
	PRINT_FUNC;
}

void Interface::addVport(const DRW_Vport& data)
{
	PRINT_FUNC;
}

void Interface::addTextStyle(const DRW_Textstyle& data)
{
	PRINT_FUNC;
}

void Interface::addAppId(const DRW_AppId& data)
{
	PRINT_FUNC;
}

void Interface::addBlock(const DRW_Block& data)
{
	PRINT_FUNC;
	m_importer.startBlock();
}

void Interface::setBlock(const int handle)
{
	PRINT_FUNC;
}

void Interface::endBlock()
{
	PRINT_FUNC;
	m_importer.endBlock();
}

void Interface::addPoint(const DRW_Point& data)
{
	PRINT_FUNC;
	m_importer.processEntity(data);
}

void Interface::addLine(const DRW_Line& data)
{
	PRINT_FUNC;
	m_importer.processEntity(data);
}

void Interface::addRay(const DRW_Ray& data)
{
	PRINT_FUNC;
}

void Interface::addXline(const DRW_Xline& data)
{
	PRINT_FUNC;
}

void Interface::addArc(const DRW_Arc& data)
{
	PRINT_FUNC;
	m_importer.processEntity(data);
}

void Interface::addCircle(const DRW_Circle& data)
{
	PRINT_FUNC;
	m_importer.processEntity(data);
}

void Interface::addEllipse(const DRW_Ellipse& data)
{
	PRINT_FUNC;
}

void Interface::addLWPolyline(const DRW_LWPolyline& data)
{
	PRINT_FUNC;
	m_importer.processEntity(data);
}

void Interface::addPolyline(const DRW_Polyline& data)
{
	PRINT_FUNC;
}

void Interface::addSpline(const DRW_Spline* data)
{
	PRINT_FUNC;
	m_importer.processEntity(*data);
}

void Interface::addKnot(const DRW_Entity& data)
{
	PRINT_FUNC;
}

void Interface::addInsert(const DRW_Insert& data)
{
	PRINT_FUNC;
}

void Interface::addTrace(const DRW_Trace& data)
{
	PRINT_FUNC;
}

void Interface::add3dFace(const DRW_3Dface& data)
{
	PRINT_FUNC;
}

void Interface::addSolid(const DRW_Solid& data)
{
	PRINT_FUNC;
}

void Interface::addMText(const DRW_MText& data)
{
	PRINT_FUNC;
}

void Interface::addText(const DRW_Text& data)
{
	PRINT_FUNC;
}

void Interface::addDimAlign(const DRW_DimAligned *data)
{
	PRINT_FUNC;
}

void Interface::addDimLinear(const DRW_DimLinear *data)
{
	PRINT_FUNC;
}

void Interface::addDimRadial(const DRW_DimRadial *data)
{
	PRINT_FUNC;
}

void Interface::addDimDiametric(const DRW_DimDiametric *data)
{
	PRINT_FUNC;
}

void Interface::addDimAngular(const DRW_DimAngular *data)
{
	PRINT_FUNC;
}

void Interface::addDimAngular3P(const DRW_DimAngular3p *data)
{
	PRINT_FUNC;
}

void Interface::addDimOrdinate(const DRW_DimOrdinate *data)
{
	PRINT_FUNC;
}

void Interface::addLeader(const DRW_Leader *data)
{
	PRINT_FUNC;
}

void Interface::addHatch(const DRW_Hatch *data)
{
	PRINT_FUNC;
}
	
void Interface::addViewport(const DRW_Viewport& data)
{
	PRINT_FUNC;
}

void Interface::addImage(const DRW_Image *data)
{
	PRINT_FUNC;
}

void Interface::linkImage(const DRW_ImageDef *data)
{
	PRINT_FUNC;
}

void Interface::addComment(const char* comment)
{
	PRINT_FUNC;
}


void Interface::writeHeader(DRW_Header& data)
{
	PRINT_FUNC;
}

void Interface::writeBlocks()
{
	PRINT_FUNC;
}

void Interface::writeBlockRecords()
{
	PRINT_FUNC;
}

void Interface::writeEntities()
{
	PRINT_FUNC;
}

void Interface::writeLTypes()
{
	PRINT_FUNC;
}

void Interface::writeLayers()
{
	PRINT_FUNC;
}

void Interface::writeTextstyles()
{
	PRINT_FUNC;
}

void Interface::writeVports()
{
	PRINT_FUNC;
}

void Interface::writeDimstyles()
{
	PRINT_FUNC;
}

void Interface::writeAppId()
{
	PRINT_FUNC;
}

}

