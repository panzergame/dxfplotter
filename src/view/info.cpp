#include <info.h>
#include <view2d/viewport.h>

namespace View
{

Info::Info(View2d::Viewport *viewport)
{
	setupUi(this);

	connect(viewport, &View2d::Viewport::cursorMoved, this, &Info::cursorMoved);
}

void Info::cursorMoved(const QPointF &position)
{
	cursorX->setText(QString::number(position.x()));
	cursorY->setText(QString::number(position.y()));
}

}
