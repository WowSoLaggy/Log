#include "stdafx.h"
#include "Log.h"


namespace Log
{

	std::string Logger::m_logPath;
	std::string Logger::m_productName;


	Logger::Logger(std::string pPrefix)
	{
		m_prefix = pPrefix;
	}

	Logger::~Logger()
	{
	}

	LogErrCode Logger::Init(std::string pLogPath, std::string pProductName, std::string pFilePath)
	{
		m_productName = pProductName;
		m_logPath = pLogPath;

		std::string version;
		bool gotVersion = GetProductAndVersion(pFilePath, version);

		LOG("");

		echo("");
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		if (gotVersion)
			echo(m_productName + std::string(" v.").append(version).append(" started."));
		else
			echo(m_productName + std::string(" started."));
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		echo("");

		return logerr_noErr;
	}

	LogErrCode Logger::Dispose()
	{
		LOG("");

		echo("");
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		echo(m_productName + std::string(" stopped."));
		echo("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		echo("");

		return logerr_noErr;
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

		m_fOut.open(m_logPath, std::ios::app);
		if (m_prefix.empty())
			m_fOut << TimeStr << " | " << pText << std::endl;
		else
			m_fOut << TimeStr << " | (" << m_prefix << ") " << pText << std::endl;
		m_fOut.close();
	}

} // Log
