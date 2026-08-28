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

#ifndef ENGINEOPTION_H
#define ENGINEOPTION_H

#include <QString>
#include <QVariant>
#include <QStringList>

class LIB_EXPORT EngineOption
{
	public:
		enum Type
		{
			CheckType,
			ComboType,
			SpinType,
			TextType,
			ButtonType
		};

		enum TextEditorType
		{
			LineEdit,
			FileDialog,
			FolderDialog
		};

		explicit EngineOption(Type type,
				      const QString& name,
				      QVariant::Type valueType,
				      const QVariant& value = QVariant(),
				      const QVariant& defaultValue = QVariant(),
				      const QString& alias = QString());
		EngineOption(const EngineOption& other);
		EngineOption& operator=(const EngineOption& other);
		~EngineOption();

		/*! Creates and returns a deep copy of this option. */
		EngineOption* copy() const;

		Type type() const;
		QVariant::Type valueType() const;
		bool isValid() const;
		bool isValid(const QVariant& value) const;
		bool isEditable() const;

		QString name() const;
		QVariant value() const;
		QVariant defaultValue() const;
		QString alias() const;

		void setName(const QString& name);
		void setValue(const QVariant& value);
		void setDefaultValue(const QVariant& value);
		void setAlias(const QString& alias);

		// Type-specific accessors (only meaningful for the relevant type)
		QStringList choices() const;
		void setChoices(const QStringList& choices);
		int min() const;
		int max() const;
		void setMin(int min);
		void setMax(int max);
		TextEditorType textEditorType() const;
		void setTextEditorType(TextEditorType editorType);

		QVariant toVariant() const;

	private:
		Type m_type;
		QVariant::Type m_valueType;
		QString m_name;
		QVariant m_value;
		QVariant m_defaultValue;
		QString m_alias;
		QStringList m_choices;
		int m_min;
		int m_max;
		TextEditorType m_editorType;
};

#endif // ENGINEOPTION_H
