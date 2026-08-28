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

#include "engineoptionfactory.h"

#include "engineoption.h"

EngineOption* EngineOptionFactory::create(const QVariantMap& map)
{
	// TODO: use isValid() for each option

	const QString name = map["name"].toString();
	const QString type = map["type"].toString();
	const QVariant value = map["value"];
	QVariant defaultValue = map["default"];
	const QString alias = map["alias"].toString();

	if (name.isEmpty())
	{
		qWarning("Empty option name");
		return nullptr;
	}

	// Special case for the button option type: its value is the name
	if (type == "button")
		return new EngineOption(EngineOption::ButtonType, name, QVariant::Invalid);

	if (value.type() != QVariant::Bool &&
		value.type() != QVariant::String &&
		value.type() != QVariant::Int)
	{
		qWarning("Invalid value type for option: %s",
			 qUtf8Printable(name));
		return nullptr;
	}

	if (defaultValue.isNull())
		defaultValue = value;
	else if (defaultValue.type() != QVariant::Bool &&
		defaultValue.type() != QVariant::String &&
		defaultValue.type() != QVariant::Int)
	{
		qWarning("Invalid default value type for option: %s",
			 qUtf8Printable(name));
		return nullptr;
	}

	// If the option type has not been defined, use text option as
	// a default option type
	if (type.isEmpty())
	{
		return new EngineOption(EngineOption::TextType, name,
			QVariant::String, value.toString(),
			defaultValue.toString(), alias);
	}
	else if (type == "text" || type == "file" || type == "folder")
	{
		EngineOption::TextEditorType editorType = EngineOption::LineEdit;
		if (type == "file")
			editorType = EngineOption::FileDialog;
		else if (type == "folder")
			editorType = EngineOption::FolderDialog;

		EngineOption* option = new EngineOption(EngineOption::TextType,
			name, QVariant::String, value.toString(),
			defaultValue.toString(), alias);
		option->setTextEditorType(editorType);
		return option;
	}
	else if (type == "check")
	{
		return new EngineOption(EngineOption::CheckType, name,
			QVariant::Bool, value.toBool(),
			defaultValue.toBool(), alias);
	}
	else if (type == "combo")
	{
		const QStringList choices(map["choices"].toStringList());
		if (choices.isEmpty())
			return nullptr;

		EngineOption* option = new EngineOption(EngineOption::ComboType,
			name, QVariant::String, value.toString(),
			defaultValue.toString(), alias);
		option->setChoices(choices);
		return option;
	}
	else if (type == "spin")
	{
		int intValue, defaultIntValue, minValue, maxValue;
		bool ok;

		intValue = value.toInt(&ok);
		if (!ok)
			return nullptr;

		defaultIntValue = defaultValue.toInt(&ok);
		if (!ok)
			return nullptr;

		minValue = map["min"].toInt(&ok);
		if (!ok)
			return nullptr;

		maxValue = map["max"].toInt(&ok);
		if (!ok)
			return nullptr;

		EngineOption* option = new EngineOption(EngineOption::SpinType,
			name, QVariant::Int, intValue, defaultIntValue, alias);
		option->setMin(minValue);
		option->setMax(maxValue);
		return option;
	}

	return nullptr;
}
