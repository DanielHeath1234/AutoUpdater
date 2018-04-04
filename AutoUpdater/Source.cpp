#include <iostream>
#include "AutoUpdater.h"

int main()
{
	auto Updater = new AutoUpdater(Version("1.0.3a"), // Current Project Release Version. // TODO: Look into char array in Version construction
		"https://raw.githubusercontent.com/DanielHeath1234/AutoUpdater/master/version"); // Download link to file containing raw version number.

	system("pause");

	delete Updater;
	return 0;
}