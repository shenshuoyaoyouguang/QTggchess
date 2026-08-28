/*
    This file is part of Cute Chess.
    Copyright (C) 2008-2018 Cute Chess authors

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "engineoption.h"


EngineOption::EngineOption(Type type,
			   const QString& name,
			   QVariant::Type valueType,
			   const QVariant& value,
			   const QVariant& defaultValue,
			   const QString& alias)
	: m_type(type),
	  m_valueType(valueType),
	  m_name(name),
	  m_value(value),
	  m_defaultValue(defaultValue),
	  m_alias(alias),
	  m_min(0),
	  m_max(0),
	  m_editorType(LineEdit)
{
	if (valueType != QVariant::Invalid)
	{
		m_value.convert(valueType);
		m_defaultValue.convert(valueType);
	}
}

EngineOption::EngineOption(const EngineOption& other)
	: m_type(other.m_type),
	  m_valueType(other.m_valueType),
	  m_name(other.m_name),
	  m_value(other.m_value),
	  m_defaultValue(other.m_defaultValue),
	  m_alias(other.m_alias),
	  m_choices(other.m_choices),
	  m_min(other.m_min),
	  m_max(other.m_max),
	  m_editorType(other.m_editorType)
{
}

EngineOption& EngineOption::operator=(const EngineOption& other)
{
	if (this != &other)
	{
		m_type = other.m_type;
		m_valueType = other.m_valueType;
		m_name = other.m_name;
		m_value = other.m_value;
		m_defaultValue = other.m_defaultValue;
		m_alias = other.m_alias;
		m_choices = other.m_choices;
		m_min = other.m_min;
		m_max = other.m_max;
		m_editorType = other.m_editorType;
	}
	return *this;
}

EngineOption::~EngineOption()
{
}

EngineOption* EngineOption::copy() const
{
	return new EngineOption(*this);
}

EngineOption::Type EngineOption::type() const
{
	return m_type;
}

QVariant::Type EngineOption::valueType() const
{
	return m_valueType;
}

bool EngineOption::isValid() const
{
	if (m_name.isEmpty())
		return false;
	if (!isValid(m_value))
		return false;
	if (!m_defaultValue.isNull() && !isValid(m_defaultValue))
		return false;

	return true;
}

bool EngineOption::isValid(const QVariant& value) const
{
	switch (m_type)
	{
		case CheckType:
			if (value.canConvert(QVariant::Bool))
			{
				QString str(value.toString());
				return (str == "true" || str == "false");
			}
			return false;

		case ComboType:
			return m_choices.contains(value.toString());

		case SpinType:
		{
			if (m_min > m_max)
				return false;

			bool ok = false;
			int tmp = value.toInt(&ok);
			if (!ok
			||  ((m_min != 0 || m_max != 0) && (tmp < m_min || tmp > m_max)))
				return false;

			return true;
		}

		case TextType:
			return value.canConvert(QVariant::String);

		case ButtonType:
			return value.isNull();
	}

	return false;
}

bool EngineOption::isEditable() const
{
	return m_type != ButtonType;
}

QString EngineOption::name() const
{
	return m_name;
}

QVariant EngineOption::value() const
{
	return m_value;
}

QVariant EngineOption::defaultValue() const
{
	return m_defaultValue;
}

QString EngineOption::alias() const
{
	return m_alias;
}

void EngineOption::setName(const QString& name)
{
	m_name = name;
}

void EngineOption::setValue(const QVariant& value)
{
	m_value = value;
	m_value.convert(m_valueType);
}

void EngineOption::setDefaultValue(const QVariant& value)
{
	m_defaultValue = value;
	m_defaultValue.convert(m_valueType);
}

void EngineOption::setAlias(const QString& alias)
{
	m_alias = alias;
}

QStringList EngineOption::choices() const
{
	return m_choices;
}

void EngineOption::setChoices(const QStringList& choices)
{
	m_choices = choices;
}

int EngineOption::min() const
{
	return m_min;
}

int EngineOption::max() const
{
	return m_max;
}

void EngineOption::setMin(int min)
{
	m_min = min;
}

void EngineOption::setMax(int max)
{
	m_max = max;
}

EngineOption::TextEditorType EngineOption::textEditorType() const
{
	return m_editorType;
}

void EngineOption::setTextEditorType(TextEditorType editorType)
{
	m_editorType = editorType;
}

QVariant EngineOption::toVariant() const
{
	QVariantMap map;

	switch (m_type)
	{
		case CheckType:
			map.insert("type", "check");
			break;
		case ComboType:
			map.insert("type", "combo");
			break;
		case SpinType:
			map.insert("type", "spin");
			break;
		case TextType:
			switch (m_editorType)
			{
				case LineEdit:
					map.insert("type", "text");
					break;
				case FileDialog:
					map.insert("type", "file");
					break;
				case FolderDialog:
					map.insert("type", "folder");
					break;
			}
			break;
		case ButtonType:
			map.insert("type", "button");
			break;
	}

	map.insert("name", name());

	if (m_type != ButtonType)
	{
		map.insert("value", value());
		map.insert("default", defaultValue());
		map.insert("alias", alias());

		if (m_type == ComboType)
			map.insert("choices", choices());
		else if (m_type == SpinType)
		{
			map.insert("min", min());
			map.insert("max", max());
		}
	}

	return map;
}
