#include <iostream>
#include "AutoUpdater.h"

int main()
{
	auto Updater = new AutoUpdater();
	Updater->run();

	system("pause");

	delete Updater;
	return 0;
}