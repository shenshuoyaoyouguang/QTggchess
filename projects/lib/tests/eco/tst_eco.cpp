// tst_eco.cpp - minimal smoke test for the ECO resource init fix
// Regression guard: eco.cpp calls qInitResources_eco() inside a file-scope
// helper (ecoInitialize) instead of the original Q_INIT_RESOURCE macro,
// because the macro expanded inside an anonymous namespace and produced
// a mangled name that the linker could not resolve. If anyone reverts
// that change, this test fails because the resource file is missing and
// ecoLookup() returns an empty EcoData for an obviously-valid SAN string.
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <cstdio>
#include <cstdlib>

#include "eco.h"

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	if (!ecoInitialize())
	{
		std::fprintf(stderr, "FAIL: ecoInitialize() returned false; "
			"resource file :/eco.bin was not embedded correctly.\n");
		return 1;
	}

	// "e4 e5 Nf3 Nc6 Bb5" is the opening of Ruy Lopez (C60).
	// If the resource was loaded correctly, ecoLookup will return a
	// non-empty ECO code and opening name.
	const EcoData data = ecoLookup("e4 e5 Nf3 Nc6 Bb5");
	if (data.eco.isEmpty() || data.opening.isEmpty())
	{
		std::fprintf(stderr,
			"FAIL: ecoLookup returned empty data. eco='%s' opening='%s'\n",
			qPrintable(data.eco), qPrintable(data.opening));
		return 2;
	}

	std::printf("OK: ECO='%s' opening='%s' variation='%s'\n",
		qPrintable(data.eco), qPrintable(data.opening),
		qPrintable(data.variation));
	return 0;
}
