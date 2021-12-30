#pragma once

#include <uic/dialogs/ui_mirror.h>

#include <QDialog>

#include <Eigen/Dense>

namespace view::dialogs
{

class Mirror : public QDialog, private Ui::Mirror
{
private:
	Eigen::Affine2d m_matrix;

public:
	explicit Mirror();

	const Eigen::Affine2d &matrix() const;

	void accept() override;
};

}
