#include <iostream>
#include "AutoUpdater.h"

int main()
{
	auto Updater = new AutoUpdater(1.0f, // Current Project Release Version.
		"https://raw.githubusercontent.com/DanielHeath1234/AutoUpdater/master/version"); // Download link to file containing raw version number.

	system("pause");

	delete Updater;
	return 0;
}