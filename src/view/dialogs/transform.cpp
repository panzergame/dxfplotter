#include <transform.h>


namespace view::dialogs
{

Transform::Transform()
{
	setupUi(this);
}

const QTransform &Transform::matrix() const
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
