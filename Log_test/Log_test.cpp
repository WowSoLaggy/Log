#include "stdafx.h"
#include "CppUnitTest.h"

#include "Log.h"


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

		TEST_METHOD(CheckLogNoInit)
		{
			try
			{
				LOG("CheckLogNoInit()");
				echo("Hello, world!");
			}
			catch (...)
			{
				Assert::Fail(L"Exception thrown.");
			}

			Logger::WriteMessage("No exception thrown.");
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

			std::string funcName = "Foo::Bar()";
			std::string funcNameRegex = "Foo::Bar\\(\\)";
			std::string testMessage1 = "Hello, world!";
			std::string testMessage2 = ". One more test message";
			std::string testMessage3 = ". And the last one.";
			LOG(funcName);
			echo(testMessage1, testMessage2, testMessage3);
			Logger::WriteMessage("Echoed...");

			std::string testLine;
			std::fstream file(logFileName, std::ios::in);
			for (int i = 0; i < 6; ++i)
				std::getline(file, testLine);
			file.close();

			// testLine should contain something like this:
			// 2016.03.16 19:49:45 > Foo::Bar(): Hello, world!
			std::regex regex(std::string("\\d{4}\\.\\d{2}\\.\\d{2} \\d{2}:\\d{2}:\\d{2} . ").append(funcNameRegex).append(": ").
				append(testMessage1).append(testMessage2).append(testMessage3));
			bool res = std::regex_match(testLine, regex);

			// Cleanup after ourselves
			LOGDISPOSE;
			DeleteFile(logFileName.c_str());

			Assert::IsTrue(res, L"Log output to file is invalid.");
			Logger::WriteMessage("Log output to file is valid.");
		}

		TEST_METHOD(CheckLogRotate_NoSize)
		{
			std::string logFileName = "Log.log";
			std::string logFileName0 = "Log.log.0";
			std::string logFileName1 = "Log.log.1";
			DeleteFile(logFileName.c_str());
			DeleteFile(logFileName0.c_str());
			DeleteFile(logFileName1.c_str());

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", -1);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file was not created.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName0), L"Unnecessary log file .0 was created.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName1), L"Unnecessary log file .1 was created.");
			LOGDISPOSE;

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", -1);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file disappeared.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName0), L"Unnecessary log file .0 was created.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName1), L"Unnecessary log file .1 was created.");
			LOGDISPOSE;

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", 0);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file disappeared.");
			Assert::IsTrue(Log::CheckFileExists(logFileName0), L"New log file .0 was not created.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName1), L"Unnecessary log file .1 was created.");
			LOGDISPOSE;

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", -1);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file disappeared.");
			Assert::IsTrue(Log::CheckFileExists(logFileName0), L"New log file .0 was not created.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName1), L"Unnecessary log file .1 was created.");
			LOGDISPOSE;

			DeleteFile(logFileName.c_str());
			DeleteFile(logFileName0.c_str());
		}

		TEST_METHOD(CheckLogRotate_WithSize)
		{
			std::string logFileName = "Log.log";
			std::string logFileName0 = "Log.log.0";
			DeleteFile(logFileName.c_str());
			DeleteFile(logFileName0.c_str());

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", 2000);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file was not created.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName0), L"Unnecessary log file .0 was created.");
			LOGDISPOSE;

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", 2000);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file disappeared.");
			Assert::IsTrue(!Log::CheckFileExists(logFileName0), L"Unnecessary log file .0 was created.");
			LOG("Test::Test()");
			std::string str(2048, 'q');
			echo(str);
			LOGDISPOSE;

			LOGINIT_ROTATE(logFileName, "Logger", "Log_test.dll", 2000);
			Assert::IsTrue(Log::CheckFileExists(logFileName), L"Log file disappeared.");
			Assert::IsTrue(Log::CheckFileExists(logFileName0), L"New log file .0 was not created.");
			LOGDISPOSE;

			DeleteFile(logFileName.c_str());
			DeleteFile(logFileName0.c_str());
		}

	};

} // Log_test
