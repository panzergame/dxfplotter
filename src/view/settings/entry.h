#pragma once

#include <QDoubleSpinBox>
#include <QLineEdit>

#include <config/property.h>
#include <common/aggregable.h>

namespace View::Settings
{

/** @brief Interface for setting entry, expose saving to configuration method
 */
class IEntry : public Common::Aggregable<IEntry>
{
public:
	/// Save entry value to config property
	virtual void save() const = 0;
};

template <class PropertyType>
class Entry : public IEntry
{
};

template <>
class Entry<float> : public QDoubleSpinBox, public IEntry
{
private:
	Config::Property<float> &m_property;

public:
	explicit Entry(Config::Property<float> &property, QWidget *parent)
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
class Entry<int> : public QSpinBox, public IEntry
{
private:
	Config::Property<int> &m_property;

public:
	explicit Entry(Config::Property<int> &property, QWidget *parent)
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
class Entry<std::string> : public QLineEdit, public IEntry
{
private:
	Config::Property<std::string> &m_property;

public:
	explicit Entry(Config::Property<std::string> &property, QWidget *parent)
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
