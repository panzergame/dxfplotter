#pragma once

#include <ui_viewport.h>

#include <QGraphicsView>

namespace View
{

class Viewport : public QGraphicsView, private Ui::Viewport
{
public:
	explicit Viewport(const PathsPtr &paths);
};

}
