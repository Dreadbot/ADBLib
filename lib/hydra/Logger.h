#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <sstream>
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::stringstream;

//Logger files were ripped out of a game dev library. Some things here (such as flags) are customized more for game dev than for robotics.
//Note that as the roboRIO is Linux-based (or something really close) a "/" needs to be at the beginning of the filename in order to log anything.
namespace Hydra
{
	#define MAX_LOGBUFFER_ENTRIES 10 //How many entries the log buffer stores before outputting all entries to a file. Change this to 0 for unstable (i.e. can crash) environments.

	enum logFlag {error, hydsys, info, resource}; //!< All possible flags that could be used. Default is hydsys.

	/**
	 * @brief Simple class for a single, independent log file that can be passed around normally.
	 */
	class Log
	{
	public:
		Log(string newName, string newFilename);
		void log(string message, logFlag flag = hydsys);
		void flushBuffer();
	private:
		vector<string> logBuffer;
		string filename;
		string name;
		friend class Logger;
	};

	/**
	 * @brief Singleton logging class allowing for global access to logging capabilities without a whole lot of annoyances.
	 */
	class Logger
	{
	public:
		void log(string message = "Default log output", logFlag flag = hydsys, string name = "sysLog");
		void newLog(string name = "sysLog", string filename = "/sysLog");
		Log* getLog(string name);
		void flushLogBuffers();
		static Logger* getInstance();
	private:
		friend class Log;
		static Logger* instance;
		vector<Log> logFiles;
		Logger();
		~Logger();
	};
};
