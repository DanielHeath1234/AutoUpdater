#include "AutoUpdater.h"

#include <curl/curl.h>
#include <iostream>
#include <iomanip>
//#include <sstream>
#include <cassert>

using std::string;

AutoUpdater::AutoUpdater(Version cur_version, const string version_url, const string download_url) : m_version(&cur_version)
{
	// Converts const string into char array for use in CURL.
	strncpy_s(m_versionURL, (char*)version_url.c_str(), sizeof(m_versionURL));
	strncpy_s(m_downloadURL, (char*)download_url.c_str(), sizeof(m_downloadURL));

	// Runs the updater upon construction.
	run();
}

AutoUpdater::~AutoUpdater()
{

}

void AutoUpdater::run()
{
	// Keep .0 on the end of float when outputting.
	std::cout << std::fixed << std::setprecision(1);

	if(downloadVersionNumber())
	{
		// Version number downloaded and handled correctly.

		if (checkForUpdate()) 
		{
			// Update available.
			// TODO: Launch GUI
		}
		else
		{
			// Up to date.
		}
	}
}

bool AutoUpdater::downloadVersionNumber()
{
	CURL *curl;
	CURLcode res;
	string readBuffer;

	curl = curl_easy_init();
	if (curl)
	{
		// Download version number from file.
		// TODO: Will this handle other types of URLs? 
		// pastebin, random website etc
		curl_easy_setopt(curl, CURLOPT_URL, m_versionURL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		// Attempt to initalise downloaded version string as type Version.
		try
		{
			for (size_t i = 0; i < readBuffer.size(); i++)
			{
				// Replace new-line with null-terminator.
				if (readBuffer[i] == '\n')
				{
					readBuffer[i] = '\0';
					break;
				}
			}
			m_newVersion = new Version(readBuffer);
			return true;
		}
		catch (...)
		{
			// TODO: Better error handling.

			// Should only be a developer issue due to version string
			// being incorrect on file or version_url isn't valid / downloading
			// the wrong thing.
			assert("Failed to initalise version string as type Version or invalid version url.");

			return false;
		}
	}
	return false;
}

bool AutoUpdater::checkForUpdate()
{
	// Checks for differences in versions.
	if (m_version->operator<(*m_newVersion))
	{
		// An update is available.
		std::cout << "An Update is Available." << std::endl
			<< "Newest Version: " << m_newVersion->getVersionString() << std::endl
			<< "Current Version: " << m_version->getVersionString() << std::endl << std::endl;
		return true;
	}

	// The versions are either equal or downloaded version string is < current.
	std::cout << "Your project is up to date." << std::endl
		<< "Downloaded Version: " << m_newVersion->getVersionString() << std::endl
		<< "Current Version: " << m_version->getVersionString() << std::endl << std::endl;
	return false;
}

size_t AutoUpdater::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
