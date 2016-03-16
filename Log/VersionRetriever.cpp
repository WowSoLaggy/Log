#include "stdafx.h"
#include "VersionRetriever.h"


namespace Log
{

	bool GetProductVersion(std::string pFilePath, std::string &strProductVersion)
	{
		// Allocate a block of memory for the version info
		unsigned long dummy;
		unsigned long dwSize = GetFileVersionInfoSize(pFilePath.c_str(), &dummy);
		if (dwSize == 0)
		{
			//TRACE("GetFileVersionInfoSize failed with error %d\n", GetLastError());
			DWORD err = GetLastError();
			return false;
		}
		std::vector<unsigned char> data(dwSize);

		// Load the version info
		if (!GetFileVersionInfo(pFilePath.c_str(), NULL, dwSize, &data[0]))
		{
			//TRACE("GetFileVersionInfo failed with error %d\n", GetLastError());
			return false;
		}

		// Get the name and version strings
		LPVOID ptrVersion = NULL;
		unsigned int iVersion = 0;

		VerQueryValue(&data[0], ("\\"), &ptrVersion, &iVersion);
		if (iVersion == 0)
			return false;

		VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)ptrVersion;
		if (verInfo->dwSignature != 0xfeef04bd)
			return false;

		// Doesn't matter if you are on 32 bit or 64 bit,
		// DWORD is always 32 bits, so first two revision numbers
		// come from dwFileVersionMS, last two come from dwFileVersionLS
		strProductVersion.clear();
		strProductVersion.
			append(std::to_string((verInfo->dwProductVersionMS >> 16) & 0xffff).append(".").
				append(std::to_string((verInfo->dwProductVersionMS >> 0) & 0xffff)).append(".").
				append(std::to_string((verInfo->dwProductVersionLS >> 16) & 0xffff)).append(".").
				append(std::to_string((verInfo->dwProductVersionLS >> 0) & 0xffff)));

		return true;
	}

} // Log
