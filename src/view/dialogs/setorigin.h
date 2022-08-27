#pragma once

#include <uic/dialogs/ui_setorigin.h>

#include <QDialog>
#include <QButtonGroup>

#include <geometry/rect.h>

namespace view::dialogs
{

class SetOrigin : public QDialog, private Ui::SetOrigin
{
private:
	const geometry::Rect &m_selectionBoundingRect;
	QTransform m_matrix;

	QButtonGroup m_buttonGroup;

	enum class Corner {
		BottomLeft = 0,
		TopLeft,
		BottomRight,
		TopRight
	};

	void setupButtonGroup();
	QVector2D boundingRectCornerPosition(Corner corner) const;

public:
	explicit SetOrigin(const geometry::Rect &selectionBoundingRect);

	const QTransform &matrix() const;

	void accept() override;
};

}
