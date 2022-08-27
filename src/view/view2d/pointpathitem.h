#pragma once

#include <view/view2d/basicpathitem.h>
#include <view/view2d/offsettedpolylinepathitem.h>

namespace view::view2d
{

/** @brief Graphics path item meant to display polylines with length.
 */
class PointPathItem : public BasicPathItem
{
	Q_OBJECT;

private:
	QPainterPath shapePath() const;

	void setupPosition();

public:
	explicit PointPathItem(model::Path &path);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


protected:
	void basePolylineTransformed() override;
};

}
