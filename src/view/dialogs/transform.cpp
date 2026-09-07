module;

#include <uic/dialogs/ui_transform.h>
#include <QDialog>

export module view.dialogs.transform;

export namespace view::dialogs
{

class Transform : public QDialog, private Ui::Transform
{
private:
	QTransform m_matrix;

public:
	explicit Transform();

	const QTransform& matrix() const;

	void accept() override;
};

}

namespace view::dialogs
{

Transform::Transform()
{
	setupUi(this);
}

const QTransform& Transform::matrix() const
{
	return m_matrix;
}

void Transform::accept()
{
	QDialog::accept();

	m_matrix.translate(offsetXSpinBox->value(), offsetYSpinBox->value());
	m_matrix.rotate(angleSpinBox->value());
}

}
