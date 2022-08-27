#include <setorigin.h>


namespace view::dialogs
{

void SetOrigin::setupButtonGroup()
{
	m_buttonGroup.addButton(topLeftCheckBox, static_cast<int>(Corner::TopLeft));
	m_buttonGroup.addButton(bottomLeftCheckBox, static_cast<int>(Corner::BottomLeft));
	m_buttonGroup.addButton(topRightCheckBox, static_cast<int>(Corner::TopRight));
	m_buttonGroup.addButton(bottomRightCheckBox, static_cast<int>(Corner::BottomRight));
}

QVector2D SetOrigin::boundingRectCornerPosition(Corner corner) const
{
	switch (corner) {
		case Corner::TopLeft:
			return m_selectionBoundingRect.topLeft();
		case Corner::BottomLeft:
			return m_selectionBoundingRect.bottomLeft();
		case Corner::TopRight:
			return m_selectionBoundingRect.topRight();
		case Corner::BottomRight:
		default:
			return m_selectionBoundingRect.bottomRight();
	}
}

SetOrigin::SetOrigin(const geometry::Rect &selectionBoundingRect)
	:m_selectionBoundingRect(selectionBoundingRect)
{
	setupUi(this);

	setupButtonGroup();
}

const QTransform &SetOrigin::matrix() const
{
	return m_matrix;
}

void SetOrigin::accept()
{
	QDialog::accept();

	const int buttonId = m_buttonGroup.checkedId();
	const Corner corner = static_cast<Corner>(buttonId);
	const QVector2D offset = boundingRectCornerPosition(corner);

	m_matrix.translate(-offset.x(), -offset.y());
}

}
