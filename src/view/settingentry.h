#pragma once

#include <QDoubleSpinBox>
#include <QLineEdit>

#include <config/variable.h>
#include <common/aggregable.h>

namespace View
{

/** @brief Interface for setting entry, expose saving to configuration method
 */
class ISettingEntry : public Common::Aggregable<ISettingEntry>
{
protected:
	Config::Variable &m_variable;

public:
	explicit ISettingEntry(Config::Variable &variable)
		:m_variable(variable)
	{
	}

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
public:
	explicit SettingEntry(Config::Variable &variable, QWidget *parent)
		:QDoubleSpinBox(parent),
		ISettingEntry(variable)
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
class SettingEntry<int> : public QSpinBox, public ISettingEntry
{
public:
	explicit SettingEntry(Config::Variable &variable, QWidget *parent)
		:QSpinBox(parent),
		ISettingEntry(variable)
	{
		setValue(m_variable);
	}

	void save() const override
	{
		m_variable = value();
	}
};

template <>
class SettingEntry<std::string> : public QLineEdit, public ISettingEntry
{
public:
	explicit SettingEntry(Config::Variable &variable, QWidget *parent)
		:QLineEdit(parent),
		ISettingEntry(variable)
	{
		setText(QString::fromStdString(m_variable));
	}

	void save() const override
	{
		m_variable = text().toStdString();
	}
};

}
