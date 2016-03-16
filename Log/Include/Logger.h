#pragma once

#ifndef LOG_H
#define LOG_H


#include "LogErrCodes.h"
#include "VersionRetriever.h"


#define LOGINIT(logPath, productName, fileName) Log::Logger::Init(logPath, productName, fileName)
#define LOG(prefix) Log::Logger log(prefix)
#define echo(text) log.Echo(text)


namespace Log
{

	class Logger
	{
	public:

		Logger(std::string pPrefix);
		~Logger();

		void Echo(std::string pText);

		static LogErrCode Init(std::string pLogPath, std::string pProductName, std::string pFilePath); // pFilePath is file to retrieve version from
		static LogErrCode Dispose();

	private:

		static std::string m_productName;
		std::string m_prefix;
		std::fstream m_fOut;

		static std::string m_logPath;
	};

} // Log


#endif // LOG_H
