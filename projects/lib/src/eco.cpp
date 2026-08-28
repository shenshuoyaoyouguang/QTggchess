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
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "eco.h"
#include <QFile>
#include <QDataStream>
#include <QMap>
#include <QMutex>
#include <QStringList>

namespace {

QStringList s_openings;
class EcoNode;

EcoNode* s_root = nullptr;
QMutex s_mutex;

class EcoDeleter
{
	public:
		~EcoDeleter() { delete s_root; }
};
EcoDeleter s_ecoDeleter;

class EcoNode
{
	public:
		EcoNode() : m_ecoCode(-1), m_opening(-1) {}
		~EcoNode() { qDeleteAll(m_children); }

		EcoNode* child(const QString& san) const { return m_children.value(san); }
		bool isLeaf() const { return m_ecoCode != -1; }
		QString ecoCode() const
		{
			if (m_ecoCode == -1) return QString();
			return QChar('A' + m_ecoCode / 100)
			     + QString("%1").arg(m_ecoCode % 100, 2, 10, QChar('0'));
		}
		QString opening() const
		{
			return m_opening >= 0 ? s_openings[m_opening] : QString();
		}
		QString variation() const { return m_variation; }

		friend QDataStream& operator>>(QDataStream& in, EcoNode*& node);

	private:
		qint16 m_ecoCode;
		qint32 m_opening;
		QString m_variation;
		QMap<QString, EcoNode*> m_children;
};

QDataStream& operator>>(QDataStream& in, EcoNode*& node)
{
	node = new EcoNode;
	in >> node->m_ecoCode
	   >> node->m_opening
	   >> node->m_variation
	   >> node->m_children;
	return in;
}

// ponytail: duplicated from old econode.cpp; keep one global ECO tree in memory.
// Upgrade path: split per-thread tree if ECO lookups become a contention point.
EcoNode* loadRoot()
{
	Q_INIT_RESOURCE(eco);

	QFile file(":/eco.bin");
	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning("Could not open ECO file");
		return nullptr;
	}

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_6);
	EcoNode* root = nullptr;
	in >> s_openings >> root;
	return root;
}

} // anonymous namespace

bool ecoInitialize()
{
	QMutexLocker locker(&s_mutex);
	if (s_root)
		return true;
	s_root = loadRoot();
	return s_root != nullptr;
}

// ponytail: by-value to keep callers thread-safe without a lock per call.
// Upstream Econode returned a tree pointer; the lookup was effectively a
// read-only tree traversal that didn't need copying. The new linear SAN
// scan reads a small static scratch buffer, so a per-call copy is cheaper
// than a mutex on every PgnGame::addMove (one of the hottest paths).
EcoData ecoLookup(const QString& sanSequence)
{
	if (sanSequence.isEmpty())
		return EcoData();

	if (!ecoInitialize())
		return EcoData();

	EcoNode* current = s_root;
	EcoNode* best = nullptr;
	const QStringList tokens = sanSequence.split(' ', Qt::SkipEmptyParts);

	for (const QString& san : tokens)
	{
		EcoNode* next = current->child(san);
		if (!next)
			break;
		if (!next->opening().isEmpty())
			best = next;
		current = next;
	}

	if (!best)
		return EcoData();

	EcoData out;
	out.eco = best->ecoCode();
	out.opening = best->opening();
	out.variation = best->variation();
	return out;
}
