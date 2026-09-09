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
	explicit Mirror() { setupUi(this); }

	const QTransform& matrix() const { return m_matrix; }

	void accept() override
	{
		QDialog::accept();

		const float scaleX = mirrorX->isChecked() ? -1.0f : 1.0f;
		const float scaleY = mirrorY->isChecked() ? -1.0f : 1.0f;
		m_matrix.scale(scaleX, scaleY);
	}
};

}
