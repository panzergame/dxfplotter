#pragma once

#include <uic/dialogs/ui_mirror.h>

#include <QDialog>


namespace view::dialogs
{

class Mirror : public QDialog, private Ui::Mirror
{
private:
	QTransform m_matrix;

public:
	explicit Mirror();

	const QTransform &matrix() const;

	void accept() override;
};

}
