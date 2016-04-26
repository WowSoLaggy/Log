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
		bool gotVersion = false;
		std::string version = "";

		s_logMutex.lock();
		{
			if (pProductName.empty())
				s_productName = "Logging"; // To write something like "Logging started"
			else
				s_productName = pProductName;
			s_logFileName = pLogFileName;

			if ((!pFilePath.empty()) && (!pProductName.empty())) // Don't acquire version if the product name is not provided
				gotVersion = GetProductVersion(pFilePath, version);

			s_isInitialized = true;
		}
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
		{
			// Clear static vars
			s_productName.clear();
			s_logFileName.clear();

			s_isInitialized = false;
		}
		s_logMutex.unlock();
	}

	std::string Logger::GetDateTimeString()
	{
		// Get local time and convert it to struct 'tm'
		time_t Clock;
		time(&Clock);

		tm newTime;
		std::string dateTimeStr;
		dateTimeStr.resize(20);

		if (!localtime_s(&newTime, &Clock))
		{
			sprintf_s(&dateTimeStr[0],
				dateTimeStr.size(),
				"%04d.%02d.%02d %02d:%02d:%02d",
				1900 + newTime.tm_year,
				1 + newTime.tm_mon,
				newTime.tm_mday,
				newTime.tm_hour,
				newTime.tm_min,
				newTime.tm_sec);
		}
		else
			sprintf_s(&dateTimeStr[0], dateTimeStr.size(), "[unknown]");

		// Delete trailing \0 symbol
		dateTimeStr.erase(dateTimeStr.end() - 1);

		return dateTimeStr;
	}

	void Logger::EchoDateTime()
	{
		std::string dateTimeStr = GetDateTimeString();

		s_logMutex.lock();

		std::fstream m_logFile(s_logFileName, std::ios::app);
		if (m_prefix.empty())
		{
			std::cout << dateTimeStr << " > ";
			m_logFile << dateTimeStr << " > ";
		}
		else
		{
			std::cout << dateTimeStr << " > " << m_prefix << ": ";
			m_logFile << dateTimeStr << " > " << m_prefix << ": ";
		}
		m_logFile.close();
	}

} // Log
