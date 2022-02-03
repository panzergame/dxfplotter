#pragma once

#include <type_traits>

#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

#include <config/property.h>
#include <common/aggregable.h>

namespace view::settings
{

/** @brief Interface for setting entry, expose saving to configuration destructor
 */
class IEntry
{
public:
	/// Save entry value to config property
	virtual ~IEntry() = default;
};

template <class PropertyType, typename = void>
class Entry : public IEntry
{
};

template <>
class Entry<float> : public QDoubleSpinBox, public IEntry
{
private:
	config::Property<float> &m_property;

public:
	explicit Entry(config::Property<float> &property, QWidget *parent)
		:QDoubleSpinBox(parent),
		m_property(property)
	{
		setMaximum(1e8);
		setDecimals(4);
		setValue((float)m_property);
	}

	~Entry() final
	{
		m_property = (float)value();
	}
};

template <>
class Entry<int> : public QSpinBox, public IEntry
{
private:
	config::Property<int> &m_property;

public:
	explicit Entry(config::Property<int> &property, QWidget *parent)
		:QSpinBox(parent),
		m_property(property)
	{
		setValue(m_property);
	}

	~Entry() final
	{
		m_property = value();
	}
};

template <>
class Entry<std::string> : public QLineEdit, public IEntry
{
private:
	config::Property<std::string> &m_property;

public:
	explicit Entry(config::Property<std::string> &property, QWidget *parent)
		:QLineEdit(parent),
		m_property(property)
	{
		setText(QString::fromStdString(m_property));
	}

	~Entry() final
	{
		m_property = text().toStdString();
	}
};

template <>
class Entry<bool> : public QCheckBox, public IEntry
{
private:
	config::Property<bool> &m_property;

public:
	explicit Entry(config::Property<bool> &property, QWidget *parent)
		:QCheckBox(parent),
		m_property(property)
	{
		setChecked(m_property);
	}

	~Entry() final
	{
		m_property = isChecked();
	}
};

template <class EnumType>
class Entry<EnumType, std::enable_if_t<std::is_enum_v<EnumType>>> : public QComboBox, public IEntry
{
private:
	config::Property<EnumType> &m_property;

public:
	explicit Entry(config::Property<EnumType> &property, QWidget *parent)
		:QComboBox(parent),
		m_property(property)
	{
		for (EnumType item : common::enumerate::All<EnumType>()) {
			const QString name = QString::fromStdString(common::enumerate::toString(item));
			const QVariant value = QVariant::fromValue(static_cast<int>(item));
			addItem(name, value);
		}

		const QString selectedName = QString::fromStdString(common::enumerate::toString(static_cast<EnumType>(m_property)));
		setCurrentText(selectedName);
	}

	~Entry() final
	{
		m_property = static_cast<EnumType>(currentData().value<int>());
	}
};

}
