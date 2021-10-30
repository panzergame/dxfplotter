#include <info.h>
#include <view2d/viewport.h>


namespace view
{

Info::Info(view2d::Viewport *viewport)
{
	setupUi(this);

	connect(viewport, &view2d::Viewport::cursorMoved, this, &Info::cursorMoved);
}

void Info::cursorMoved(const QPointF &position)
{
	cursorX->setText(QString::number(position.x()));
	cursorY->setText(QString::number(position.y()));
}

}
