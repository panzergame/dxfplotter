#include <info.h>
#include <view2d/viewport.h>

namespace View
{

Info::Info(Viewport *viewport)
{
	setupUi(this);

	connect(viewport, &Viewport::cursorMoved, this, &Info::cursorMoved);
}

void Info::cursorMoved(const QPointF &position)
{
	cursorX->setText(QString::number(position.x()));
	cursorY->setText(QString::number(position.y()));
}

}
