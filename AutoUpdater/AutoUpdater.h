#pragma once
#include <string>

using std::string;

// Version data type for handling revision numbers
struct Version
{
public:
	Version(int a_major, int a_minor, char a_revision)
		: major(a_major), minor(a_minor)
	{
		strncpy_s(revision, (char*)a_revision, sizeof(revision));
	}
	Version(string version)
	{
		// TODO: split string into major, minor and revison
		string::size_type pos = version.find('.');
		string::size_type pos2;
		if (pos != string::npos)
		{
			major = stoi(version.substr(0, pos));
			pos2 = version.find('.', pos + 1);

			if (pos2 != string::npos)
			{
				// Contains 2 periods.
				minor = stoi(version.substr(pos+1, pos2));
				strncpy_s(revision, (char*)version.substr(pos2 + 1).c_str(), sizeof(revision));
				//revision = stoi(version.substr(pos2 + 1));
			}
			else
			{
				// Contains 1 period.
				minor = stoi(version.substr(pos + 1));
				revision[10] = -1;
			}
		}
		else
		{
			// Contains 0 periods.
			major = stoi(version);
			minor = -1;
		}
	}

	string getVersionString() 
	{ 
		if (revision[10] == -1)
		{
			return std::to_string(major) + "." + std::to_string(minor);
		}
		if (minor == -1)
		{
			return std::to_string(major);
		}
		return std::to_string(major) + "." + std::to_string(minor) + "." + revision; 
	}

	// Getters and Setters 
	// ----------------------------------------------------------------------------
	inline const int getMajor() const { return major; }
	inline const int getMinor() const { return minor; }
	inline string getRevision() { return revision; }

	inline void setMajor(const int a_major) { major = a_major; }
	inline void setMinor(const int a_minor) { minor = a_minor; }
	inline void setRevision(char *a_revision) { strncpy_s(revision, (char*)a_revision, sizeof(revision)); }
	// ----------------------------------------------------------------------------

private:
	int major;
	int minor;
	char revision[10];
};

class AutoUpdater
{
public:
	AutoUpdater(float cur_version, const string version_url);
	AutoUpdater(Version cur_version, const string version_url);
	~AutoUpdater();

	void run();
	
	void downloadVersionNumber();
	void checkForUpdate();

protected:

	Version *m_versionType;
	Version *m_newVersionType;

	char m_versionURL[256];

	float m_version;
	float m_newestVersion;

	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	
};

