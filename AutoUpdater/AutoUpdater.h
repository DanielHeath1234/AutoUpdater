#pragma once
#include <string>

using std::string;

// Version data type for handling revision numbers
struct Version
{
public:
	Version(int a_major, int a_minor, char *a_revision)
		: major(a_major), minor(a_minor)
	{
		strncpy_s(revision, (char*)a_revision, sizeof(revision));
	}
	Version(string version)
	{
		string::size_type pos = version.find('.');
		if (pos != string::npos)
		{
			major = stoi(version.substr(0, pos));
			string::size_type pos2 = version.find('.', pos + 1);

			if (pos2 != string::npos)
			{
				// Contains 2 periods.
				minor = stoi(version.substr(pos+1, pos2));
				strncpy_s(revision, (char*)version.substr(pos2 + 1).c_str(), sizeof(revision));
			}
			else
			{
				// Contains 1 period.
				minor = stoi(version.substr(pos + 1));
				revision[5] = -1;
			}
		}
		else
		{
			// Contains 0 periods.
			major = stoi(version);
			minor = -1;
		}
	}

	~Version()
	{
		
	}

	string getVersionString() 
	{ 
		if (revision[5] == -1)
		{
			return std::to_string(major) + "." + std::to_string(minor);
		}
		if (minor == -1)
		{
			return std::to_string(major);
		}
		return std::to_string(major) + "." + std::to_string(minor) + "." + revision; 
	}

	bool operator>(const Version &v)
	{
		if (major > v.major && minor > v.minor)
		{
			int i = 0;
			while (revision[i] != '\0')
			{
				if (revision[i] < v.revision[i])
				{
					return false;
				}
				i++;
			}
			return true;
		}
		return false;
	}
	bool operator<(const Version &v)
	{
		// TODO: Cunt's fucked. Get this working then straight to work on the GUI
		// Maybe do some quick mockups of ui flow?
		if (major <= v.major)
		{
			if (minor <= v.minor)
			{
				int i = 0;
				while (revision[i] != '\0' && v.revision[i] != '\0')
				{
					if (revision[i + 1] != '\0' && v.revision[i + 1] != '\0') // != eof-1
					{
						if (revision[i] < v.revision[i])
						{
							return true;
						}
					}
					else
					{
						if (revision[i] < v.revision[i])
						{
							return true;
						}
						if (revision[i] == v.revision[i] && revision[i+1] != '\0')
						{
							return false;
						}
					}
					i++;
				}
				return true;
			}
		}
		return false;
	}
	bool operator=(const Version &v)
	{
		if (major == v.major && minor == v.minor)
		{
			int i = 0;
			while (revision[i] != '\0')
			{
				if (revision[i] != v.revision[i])
				{
					return false;
				}
				i++;
			}
			return true;
		}
		return false;
	}
	bool operator<=(const Version &v)
	{
		if (major <= v.major && minor <= v.minor)
		{
			int i = 0;
			while (revision[i] != '\0')
			{		
				if (revision[i+1] != '\0' && v.revision[i+1] != '\0')
				{
					if (revision[i] > v.revision[i])
					{
						return false;
					}
				}
				else
				{
					if (revision[i] == v.revision[i])
					{
						return true;
					}
				}
				i++;
			}
			return true;
		}
		return false;
	}

	// Getters and Setters 
	// ---------------------------------------------------------
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
	char revision[5]; // Memory for a triple digit revision number, a single character and a null-terminator ('\0').
};

class AutoUpdater
{
public:
	AutoUpdater(Version cur_version, const string version_url, const string download_url = "");
	~AutoUpdater();

	void run();
	
	bool downloadVersionNumber();
	bool checkForUpdate();

private:
	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

protected:

	Version *m_version;
	Version *m_newVersion;

	char m_versionURL[256];
	char m_downloadURL[256];
};

