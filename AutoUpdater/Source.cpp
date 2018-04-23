#include <iostream>
#include "AutoUpdater.h"

int main()
{
	auto Updater = new AutoUpdater(Version("1.0.3b"), // Current Project Release Version. // TODO: Look into char array in Version construction
		"https://raw.githubusercontent.com/DanielHeath1234/AutoUpdater/master/version", // Download link to file containing a raw version number (int.int.char[]).
		"https://github.com/DanielHeath1234/AutoUpdater/archive/master.zip"); // Download link to .zip of newest version.

	delete Updater;
	return 0;
}