#include <mirror.h>


namespace view::dialogs
{

Mirror::Mirror()
{
	setupUi(this);
}

const Eigen::Affine2d &Mirror::matrix() const
{
	return m_matrix;
}

void Mirror::accept()
{
	QDialog::accept();

	const float scaleX = mirrorX->isChecked() ? -1.0f : 1.0f;
	const float scaleY = mirrorY->isChecked() ? -1.0f : 1.0f;
	m_matrix.scale(Eigen::Vector2d(scaleX, scaleY));
}

}
