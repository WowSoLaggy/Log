// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:		Logger
// Author:		Anton Egorov
// Description:	Class that logs any text to the console and the given file.
//				Thread-safety is guaranteed.
//
// Usage example:
//
// void SomeFunc()
// {
//		LOG("SomeFunc()");
//		...
//		echo("Hello, world! Result is: ", someVariable, ".");
//		...
// }
//
// void main()
// {
//		LOGINIT("Log.log", "MyProduct", "ThisFile.exe");
//		...
//		SomeFunc();
//		...
//		LOGDISPOSE();
// }
//
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

#ifndef LOG_H
#define LOG_H


// Initializes logger. Should be called first of all. Writes the welcome message to the log
// Params:
// [in] std::string pLogFileName	- log file name (ex. "Logs\myLog.log"). Warning: Directory should exist
// [in] std::string pProductName	- product name used in the welcome message
// [in] std::string pFilePath		- name of the file to take version from. Can be empty
#define LOGINIT(pLogFileName, pProductName, pFilePath) Log::Logger::Init(pLogFileName, pProductName, pFilePath)

// Disposes logger. Writes the bye message to the log
#define LOGDISPOSE Log::Logger::Dispose()

// Macros that should be called before any echo-calls. Initializes the log object for the current function
// Params:
// [in] std::string pPrefix	- function name
#define LOG(pPrefix) Log::Logger log(pPrefix)

// Logs all given args
#define echo log.Echo


namespace Log
{

	// Class that logs any text to the console and the given file
	class Logger
	{
	public:

		// Logger constructor that should be called before any echo-calls. Initializes the log object for the current function
		// Params:
		// [in] std::string pPrefix	- function name
		Logger(std::string pPrefix);

		// Logger destructor
		virtual ~Logger();


		// Logs all given args
		// Params:
		// [in] Args pArgs	- bag of args to log
		template <typename ... Args>
		void Echo(Args ... pArgs)
		{
			if (!s_isInitialized)
				return;

			EchoDateTime();
			Echo_Internal(pArgs ...);
		}


		// Initializes logger. Should be called first of all. Writes the welcome message to the log
		// Params:
		// [in] std::string pLogFileName	- log file name (ex. "Logs\myLog.log"). Warning: Directory should exist
		// [in] std::string pProductName	- product name used in the welcome message
		// [in] std::string pFilePath		- name of the file to take version from. Can be empty
		static void Init(std::string pLogFileName, std::string pProductName, std::string pFilePath);

		// Disposes logger. Writes the bye message to the log
		static void Dispose();

		// Returns the DateTime string like this: "YYYY.MM.DD HH.MM.SS"
		static std::string GetDateTimeString();

	protected:

		std::string m_prefix;					// Function name (or whatever was sent to the LOG macros)

		volatile static bool s_isInitialized;	// Indicates whether the Logger class was initialized and is ready to log
		static std::string s_productName;		// Product name. Used in the welcome and bye messages
		static std::string s_logFileName;		// Name of the file to log into
		static std::mutex s_logMutex;			// Locker to provide thread-safety of Logger operations

		void EchoDateTime();					// Logs current date and time

		// Echoes the bag of arguments
		template <typename First, typename ... Rest>
		void Echo_Internal(First pFirst, Rest ... pRest)
		{
			s_logMutex.lock();
			{
				std::fstream m_logFile(s_logFileName, std::ios::app);
				std::cout << pFirst;
				m_logFile << pFirst;
				m_logFile.close();
			}
			s_logMutex.unlock();

			Echo_Internal(pRest ...);
		}

		// Echoes the last of arguments from the args bag
		template <typename T>
		void Echo_Internal(T pPar)
		{
			s_logMutex.lock();
			{
				std::fstream m_logFile(s_logFileName, std::ios::app);
				std::cout << pPar << std::endl;
				m_logFile << pPar << std::endl;
				m_logFile.close();
			}
			s_logMutex.unlock();
		}

	};

} // Log


#endif // LOG_H
