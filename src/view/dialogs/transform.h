#pragma once

#include <uic/dialogs/ui_transform.h>

#include <QDialog>

#include <Eigen/Dense>

namespace view::dialogs
{

class Transform : public QDialog, private Ui::Transform
{
private:
	Eigen::Affine2d m_matrix;

public:
	explicit Transform();

	const Eigen::Affine2d &matrix() const;

	void accept() override;
};

}
