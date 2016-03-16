#include "stdafx.h"
#include "CppUnitTest.h"

#include "Logger.h"
#include "VersionRetriever.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Log_test
{		

	TEST_CLASS(Log_test)
	{
	public:
		
		TEST_METHOD(CheckVersionRetrieval)
		{
			std::string filePath = "Log_test.dll";
			std::string version;

			bool res = Log::GetProductVersion(filePath, version);
			
			Assert::IsTrue(res, L"Version not acquired.");
			Logger::WriteMessage("Version acquired.");

			std::regex regex("\\d\\.\\d\\.\\d\\.\\d");
			res = std::regex_match(version, regex);

			Assert::IsTrue(res, L"Version seems to be incorrect.");
			Logger::WriteMessage("Version seems to be correct.");
		}

		TEST_METHOD(CheckLogFileCreation)
		{
			std::string logFileName = "Log.log";
			DeleteFile(logFileName.c_str());
			Logger::WriteMessage("Deleted existing log file.");

			LOGINIT(logFileName, "Logger", "Log_test.dll");
			Logger::WriteMessage("Logger inited.");

			struct stat buffer;
			bool fileExists = (stat(logFileName.c_str(), &buffer) == 0);

			// Cleanup after ourselves
			LOGDISPOSE;
			DeleteFile(logFileName.c_str());

			Assert::IsTrue(fileExists, L"Log file wasn't created.");
			Logger::WriteMessage("Log file was successfully created.");
		}

		TEST_METHOD(CheckLogIsCorrect)
		{
			std::string logFileName = "Log.log";
			DeleteFile(logFileName.c_str());
			Logger::WriteMessage("Deleted existing log file.");

			LOGINIT(logFileName, "Logger", "Log_test.dll");
			Logger::WriteMessage("Logger inited.");

			std::string funcName = "Foo::Bar";
			std::string testMessage = "Hello, world!";
			LOG(funcName);
			echo(testMessage);
			Logger::WriteMessage("Echoed...");

			std::string testLine;
			std::fstream file(logFileName, std::ios::in);
			for (int i = 0; i < 6; ++i)
				std::getline(file, testLine);
			file.close();

			// testLine should contain something like this:
			// 2016.03.16 19:49:45 | (Foo::Bar) Hello, world!
			std::regex regex(std::string("\\d{4}\\.\\d{2}\\.\\d{2} \\d{2}:\\d{2}:\\d{2} \\| \\(").append(funcName).append("\\) ").append(testMessage));
			bool res = std::regex_match(testLine, regex);

			// Cleanup after ourselves
			LOGDISPOSE;
			DeleteFile(logFileName.c_str());

			Assert::IsTrue(res, L"Log output to file is invalid.");
			Logger::WriteMessage("Log output to file is valid.");
		}

	};

} // Log_test
