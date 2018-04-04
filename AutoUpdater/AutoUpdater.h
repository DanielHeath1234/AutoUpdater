#pragma once
#include <string>

using std::string;

inline string VERSION(int major, int minor, char revision) { return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision); }

class AutoUpdater
{
public:
	AutoUpdater();
	~AutoUpdater();

	void startup();
	void run();
	
	void downloadVersionNumber();
	void checkForUpdate();

protected:

	const char m_versionURL[256];

	float m_version;
	float m_newestVersion;

	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	bool isValidFloat(const string &input);
};

