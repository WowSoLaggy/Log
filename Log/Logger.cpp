#include "stdafx.h"
#include "Logger.h"

#include "VersionRetriever.h"


namespace Log
{

	volatile bool Logger::s_isInitialized = false;
	std::string Logger::s_logFileName;
	std::string Logger::s_productName;
	std::mutex Logger::s_logMutex;


	Logger::Logger(std::string pPrefix)
	{
		m_prefix = pPrefix;
	}

	Logger::~Logger()
	{
	}

	void Logger::Init(std::string pLogFileName, std::string pProductName, std::string pFilePath)
	{
		s_logMutex.lock();

		if (pProductName.empty())
			s_productName = "Logging"; // To write something like "Logging started"
		else
			s_productName = pProductName;
		s_logFileName = pLogFileName;

		std::string version;
		bool gotVersion = false;
		if ((!pFilePath.empty()) && (!pProductName.empty())) // Don't acquire version if the product name is not provided
			gotVersion = GetProductVersion(pFilePath, version);

		s_isInitialized = true;

		s_logMutex.unlock();

		// Print welcome message

		LOG("");

		echo("");
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		if (gotVersion)
			echo(s_productName + std::string(" v.").append(version).append(" started."));
		else
			echo(s_productName + std::string(" started."));
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		echo("");
	}

	void Logger::Dispose()
	{
		// Print bye message

		LOG("");

		echo("");
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		echo(s_productName + std::string(" stopped."));
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		echo("");

		s_logMutex.lock();

		// Clear static vars
		s_productName.clear();
		s_logFileName.clear();

		s_isInitialized = false;

		s_logMutex.unlock();
	}

	void Logger::Echo(std::string pText)
	{
		// Get local time and convert it to struct 'tm'
		time_t Clock;
		time(&Clock);

		tm newTime;
		char TimeStr[128];

		if (!localtime_s(&newTime, &Clock))
		{
			sprintf_s(TimeStr,
				"%04d.%02d.%02d %02d:%02d:%02d",
				1900 + newTime.tm_year,
				1 + newTime.tm_mon,
				newTime.tm_mday,
				newTime.tm_hour,
				newTime.tm_min,
				newTime.tm_sec);
		}
		else
			sprintf_s(TimeStr, "[unknown]");

		s_logMutex.lock();

		if (!s_isInitialized)
			return;

		std::fstream m_logFile(s_logFileName, std::ios::app);
		if (m_prefix.empty())
		{
			std::cout << TimeStr << " > " << pText << std::endl;
			m_logFile << TimeStr << " > " << pText << std::endl;
		}
		else
		{
			std::cout << TimeStr << " > " << m_prefix << ": " << pText << std::endl;
			m_logFile << TimeStr << " > " << m_prefix << ": " << pText << std::endl;
		}
		m_logFile.close();

		s_logMutex.unlock();
	}

} // Log
