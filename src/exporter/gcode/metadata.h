#pragma once

#include <string>

#include <config/config.h>
#include <geometry/rect.h>

namespace model
{

class Document;

}

class QJsonObject;
class QJsonArray;
class QJsonDocument;

namespace exporter::gcode
{

class Metadata
{
private:
	const geometry::Rect m_boundingRect;
	const config::Profiles::Profile::Gcode& m_gcode;
	const float m_retractDepth;

	std::string fastPlaneMoveGCode(const QVector2D &to) const;
	std::string fastDepthMoveGCode(float to) const;
	QJsonArray boundingRectGcodes() const;
	QJsonDocument toJson() const;

public:
	explicit Metadata(const model::Document& document, const config::Profiles::Profile::Gcode& gcode, float retractDepth);

	std::string toComment() const;
};

}
