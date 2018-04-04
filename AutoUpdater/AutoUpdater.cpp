#include "AutoUpdater.h"

#include <curl/curl.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

using std::string;

#define VERSION_TYPE

AutoUpdater::AutoUpdater(float cur_version, string version_url) : m_version(cur_version)
{
	#define VERSION_TYPE f;

	// Converts (string)version_url into char array.
	strncpy_s(m_versionURL, (char*)version_url.c_str(), sizeof(m_versionURL));

	// Runs the updater upon construction.
	run();
}

AutoUpdater::AutoUpdater(Version cur_version, const string version_url) : m_versionType(&cur_version)
{
	#define VERSION_TYPE v;

	// Converts (string)version_url into char array.
	strncpy_s(m_versionURL, (char*)version_url.c_str(), sizeof(m_versionURL));

	// Runs the updater upon construction.
	run();
}

AutoUpdater::~AutoUpdater()
{
	delete m_newVersionType;
	delete m_versionType;
}

void AutoUpdater::run()
{
	downloadVersionNumber();

	checkForUpdate();

	// TODO: GUI - Prompt user to update when available.
}

void AutoUpdater::downloadVersionNumber()
{
	CURL *curl;
	CURLcode res;
	string readBuffer;

	curl = curl_easy_init();
	if (curl)
	{
		// Download version number from file.
		curl_easy_setopt(curl, CURLOPT_URL, m_versionURL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		// Attempt to convert downloaded version number to a float.
		try
		{
			// TODO: Handling for different version types.
			#if VERSION_TYPE == f
				m_newestVersion = std::stof(readBuffer);
			#elif VERSION_TYPE == v
				m_newVersionType = new Version(readBuffer);
			#endif
		}
		catch (...)
		{
			// TODO: Error handling, do not continue to check for update.

			// Assert? Should only be a developer issue due to version --
			// string being incorrectly entered.

			#if VERSION_TYPE == f
				std::cerr << "Failed to convert version string to float." << std::endl
					<< "Recieved version string: " << readBuffer << std::endl;
			#elif VERSION_TYPE == v
				std::cerr << "Failed to convert version string to Version type." << std::endl
					<< "Recieved version string: " << readBuffer << std::endl;
			#endif

			return;
			
		}

		// Keep .0 on the end of float when outputting.
		std::cout << std::fixed << std::setprecision(1);
	}
}

void AutoUpdater::checkForUpdate()
{
	// Checks for differences in versions.
	if (m_newestVersion <= m_version)
	{
		// The versions are either equal or downloaded version is < current.
		std::cout << "Your project is up to date." << std::endl 
			<< "Current Version: " << m_version << std::endl << std::endl;
		return;
	}

	// An update is available.
	std::cout << "An Update is Available." << std::endl
		<< "Newest Version: " << m_newestVersion << std::endl
		<< "Current Version: " << m_version << std::endl << std::endl;
}

size_t AutoUpdater::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
