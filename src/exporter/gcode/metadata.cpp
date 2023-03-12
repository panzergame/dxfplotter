#include <metadata.h>
#include <postprocessor.h>

#include <model/document.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <sstream>

namespace exporter::gcode
{

std::string Metadata::fastPlaneMoveGCode(const QVector2D &to) const
{
	std::ostringstream output;
	PostProcessor postprocessor(m_gcode, output);
	postprocessor.fastPlaneMove(to);
	return output.str();
}

std::string Metadata::fastDepthMoveGCode(float to) const
{
	std::ostringstream output;
	PostProcessor postprocessor(m_gcode, output);
	postprocessor.retractDepth(to);
	return output.str();
}

QJsonArray Metadata::boundingRectGcodes() const
{
	std::initializer_list<std::string> gcodes{
		fastDepthMoveGCode(m_retractDepth),
		fastPlaneMoveGCode(m_boundingRect.bottomLeft()),
		fastPlaneMoveGCode(m_boundingRect.bottomRight()),
		fastPlaneMoveGCode(m_boundingRect.topRight()),
		fastPlaneMoveGCode(m_boundingRect.topLeft()),
		fastPlaneMoveGCode(m_boundingRect.bottomLeft()),
		fastPlaneMoveGCode(QVector2D(0.0f, 0.0f)),
		fastDepthMoveGCode(0.0f)
	};

	QJsonArray jsonGcodes;
	for (const std::string &gcode : gcodes) {
		jsonGcodes.push_back(QString::fromStdString(gcode));
	}

	return jsonGcodes;
}

QJsonDocument Metadata::toJson() const
{
	QJsonObject root;
	root["bounding_rect_gcodes"] = boundingRectGcodes();

	QJsonDocument document(root);
	return document;
}

Metadata::Metadata(const model::Document& document, const config::Profiles::Profile::Gcode& gcode, float retractDepth)
	:m_boundingRect(document.task().visibleBoundingRect()),
	m_gcode(gcode),
	m_retractDepth((retractDepth))
{
}

std::string Metadata::toComment() const
{
	const QJsonDocument& jsonDocument = toJson();

	return "; metadata_json = " + jsonDocument.toJson(QJsonDocument::Compact).toStdString() + "\n";
}

}
