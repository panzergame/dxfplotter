#pragma once

#include <QDoubleSpinBox>
#include <QLineEdit>

#include <config/variable.h>
#include <common/aggregable.h>

#include <QDebug> // TODO

namespace View
{

/** @brief Interface for setting entry, expose saving to configuration method
 */
class ISettingEntry : public Common::Aggregable<ISettingEntry>
{
public:
	/// Save entry value to config variable
	virtual void save() const = 0;
};

template <class VariableType>
class SettingEntry : public ISettingEntry
{
};

template <>
class SettingEntry<float> : public QDoubleSpinBox, public ISettingEntry
{
private:
	Config::Variable<float> &m_variable;

public:
	explicit SettingEntry(Config::Variable<float> &variable, QWidget *parent)
		:QDoubleSpinBox(parent),
		m_variable(variable)
	{
		setDecimals(4);
		setValue((double)m_variable);
	}

	void save() const override
	{
		m_variable = (float)value();
	}
};

template <>
class SettingEntry<std::string> : public QLineEdit, public ISettingEntry
{
private:
	Config::Variable<std::string> &m_variable;

public:
	explicit SettingEntry(Config::Variable<std::string> &variable, QWidget *parent)
		:QLineEdit(parent),
		m_variable(variable)
	{
		setText(QString::fromStdString(m_variable));
	}

	void save() const override
	{
		m_variable = text().toStdString();
	}
};

}
