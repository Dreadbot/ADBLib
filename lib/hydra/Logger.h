#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <unordered_map>
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
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
		Log() {} //!< This is never used. It has something to do with tuples. DO NOT USE THIS CONSTRUCTOR.
		void log(string message, logFlag flag = hydsys);
		void flushBuffer();
	private:
		vector<string> logBuffer;
		string filename;
		string name;
		friend class Logger;
	};

	/**
	 * @brief Static logging class allowing for global access to logging capabilities without a whole lot of annoyances.
	 */
	class Logger
	{
	public:
		Logger();
		static void log(string message, string name, logFlag flag = hydsys);
		static void newLog(string name, string filename);
		static Log* getLog(string name);
		static void flushLogBuffers();
	private:
		static unordered_map<string, Log> logFiles;
	};
};
