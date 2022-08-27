#pragma once

#include <view/view2d/basicpathitem.h>
#include <view/view2d/offsettedpolylinepathitem.h>

namespace view::view2d
{

/** @brief Graphics path item meant to display polylines with length.
 */
class PolylinePathItem : public BasicPathItem
{
	Q_OBJECT;

private:
	QPainterPath m_paintPath;
	QPainterPath m_shapePath;

	// Item of offsetted polylines of the same path.
	std::unique_ptr<OffsettedPolylinePathItem> m_offsettedPath;

	QPainterPath paintPath() const;
	static QPainterPath shapePath(const QPainterPath &basePath);

	void setupPaths();

	void updateOffsetedPath();

	// Change selected state and propagate to sub paths (e.g offsetted path).
	void setSelected(bool selected) override;

public:
	explicit PolylinePathItem(model::Path &path);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QPainterPath shape() const override;

protected:
	void basePolylineTransformed() override;
};

}
