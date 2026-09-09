module;

#include <uic/dialogs/ui_setorigin.h>
#include <QDialog>
#include <QButtonGroup>

export module view.dialogs.setorigin;

import geometry.rect;

export namespace view::dialogs
{

class SetOrigin : public QDialog, private Ui::SetOrigin
{
private:
	const geometry::Rect& m_selectionBoundingRect;
	QTransform m_matrix;

	QButtonGroup m_buttonGroup;

	enum class Corner { BottomLeft = 0, TopLeft, BottomRight, TopRight };

	void setupButtonGroup()
	{
		m_buttonGroup.addButton(topLeftCheckBox, static_cast<int>(Corner::TopLeft));
		m_buttonGroup.addButton(bottomLeftCheckBox, static_cast<int>(Corner::BottomLeft));
		m_buttonGroup.addButton(topRightCheckBox, static_cast<int>(Corner::TopRight));
		m_buttonGroup.addButton(bottomRightCheckBox, static_cast<int>(Corner::BottomRight));
	}

	QVector2D boundingRectCornerPosition(Corner corner) const
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

public:
	explicit SetOrigin(const geometry::Rect& selectionBoundingRect)
		: m_selectionBoundingRect(selectionBoundingRect)
	{
		setupUi(this);

		setupButtonGroup();
	}

	const QTransform& matrix() const { return m_matrix; }

	void accept() override
	{
		QDialog::accept();

		const int buttonId = m_buttonGroup.checkedId();
		const Corner corner = static_cast<Corner>(buttonId);
		const QVector2D offset = boundingRectCornerPosition(corner);

		m_matrix.translate(-offset.x(), -offset.y());
	}
};

}
