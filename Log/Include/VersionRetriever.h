#pragma once

#ifndef VERSIONRETRIEVER_H
#define VERSIONRETRIEVER_H


#pragma comment(lib, "version.lib")


namespace Log
{

	bool GetProductAndVersion(std::string pFilePath, std::string &strProductVersion);

} // Log


#endif // VERSIONRETRIEVER_H
