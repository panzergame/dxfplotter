#pragma once

#include <uic/dialogs/ui_transform.h>

#include <QDialog>


namespace view
{

class Transform : public QDialog, private Ui::Transform
{
private:
	QTransform m_matrix;

public:
	explicit Transform();

	const QTransform &matrix() const;

	void accept() override;
};

}
