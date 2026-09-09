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
	explicit Transform() { setupUi(this); }

	const QTransform& matrix() const { return m_matrix; }

	void accept() override
	{
		QDialog::accept();

		m_matrix.translate(offsetXSpinBox->value(), offsetYSpinBox->value());
		m_matrix.rotate(angleSpinBox->value());
	}
};

}
