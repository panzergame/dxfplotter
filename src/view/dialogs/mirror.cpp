module;

#include <uic/dialogs/ui_mirror.h>
#include <QDialog>

export module view.dialogs.mirror;

export namespace view::dialogs
{

class Mirror : public QDialog, private Ui::Mirror
{
private:
	QTransform m_matrix;

public:
	explicit Mirror();

	const QTransform& matrix() const;

	void accept() override;
};

}

namespace view::dialogs
{

Mirror::Mirror()
{
	setupUi(this);
}

const QTransform& Mirror::matrix() const
{
	return m_matrix;
}

void Mirror::accept()
{
	QDialog::accept();

	const float scaleX = mirrorX->isChecked() ? -1.0f : 1.0f;
	const float scaleY = mirrorY->isChecked() ? -1.0f : 1.0f;
	m_matrix.scale(scaleX, scaleY);
}

}
