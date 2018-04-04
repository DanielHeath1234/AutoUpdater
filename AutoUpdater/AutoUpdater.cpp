#include "AutoUpdater.h"

#include <curl/curl.h>
#include <iostream>
#include <iomanip>
#include <sstream>

using std::string;

AutoUpdater::AutoUpdater() : m_version(1.0f), m_versionURL("https://raw.githubusercontent.com/DanielHeath1234/AutoUpdater/master/version")
{

}

AutoUpdater::~AutoUpdater()
{
}

void AutoUpdater::startup()
{
	
}

void AutoUpdater::run()
{
	downloadVersionNumber();

	checkForUpdate();
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
			m_newestVersion = std::stof(readBuffer);
		}
		catch (...)
		{
			std::cout << "Failed to convert version string to float." << std::endl
				<< "Recieved version string: " << readBuffer << std::endl;
			return;
		}

		// Keep .0 on the end of float when outputting.
		std::cout << std::fixed << std::setprecision(1);

		// Outputting information to do with version numbers.
		//std::cout << "Your current version: " << m_version << std::endl;
		//std::cout << "Version read from hosted file: " << readBuffer << std::endl;
	}
}

void AutoUpdater::checkForUpdate()
{
	// Checks differences in versions.
	if (m_newestVersion <= m_version)
	{
		// The versions are either equal or newest version is < current.
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

bool AutoUpdater::isValidFloat(const string &input)
{
	
	return true;
	/*char* end = 0;
	double val = strtof(input.c_str(), &end);
	return end != input.c_str() && val != HUGE_VAL;
	/*std::istringstream iss(input);
	float f;

	// noskipws considers leading whitespace invalid
	iss >> std::noskipws >> f;

	// Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();*/
}
