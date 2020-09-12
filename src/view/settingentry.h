#pragma once

#include <QDoubleSpinBox>
#include <QLineEdit>

#include <config/property.h>
#include <common/aggregable.h>

namespace View
{

/** @brief Interface for setting entry, expose saving to configuration method
 */
class ISettingEntry : public Common::Aggregable<ISettingEntry>
{
public:
	/// Save entry value to config property
	virtual void save() const = 0;
};

template <class PropertyType>
class SettingEntry : public ISettingEntry
{
};

template <>
class SettingEntry<float> : public QDoubleSpinBox, public ISettingEntry
{
private:
	Config::Property<float> &m_property;

public:
	explicit SettingEntry(Config::Property<float> &property, QWidget *parent)
		:QDoubleSpinBox(parent),
		m_property(property)
	{
		setDecimals(4);
		setValue((float)m_property);
	}

	void save() const override
	{
		m_property = (float)value();
	}
};

template <>
class SettingEntry<int> : public QSpinBox, public ISettingEntry
{
private:
	Config::Property<int> &m_property;

public:
	explicit SettingEntry(Config::Property<int> &property, QWidget *parent)
		:QSpinBox(parent),
		m_property(property)
	{
		setValue(m_property);
	}

	void save() const override
	{
		m_property = value();
	}
};

template <>
class SettingEntry<std::string> : public QLineEdit, public ISettingEntry
{
private:
	Config::Property<std::string> &m_property;

public:
	explicit SettingEntry(Config::Property<std::string> &property, QWidget *parent)
		:QLineEdit(parent),
		m_property(property)
	{
		setText(QString::fromStdString(m_property));
	}

	void save() const override
	{
		m_property = text().toStdString();
	}
};

}
