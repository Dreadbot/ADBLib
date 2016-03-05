#include "Logger.h"
//Not sure why these should be here, they're also in Logger.h... but it won't compile without them.
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
using std::ofstream;
using std::stringstream;


namespace ADBLib
{
	/**
	 * @brief Constructor for log.
	 * @param newName the name of the log as referred to when grabbed by Logger.
	 * @param newFileName The filename for the log file.
	 * @note If on a linux-based system (such as a roboRIO), you need to add "/" to the beginning of the filename.
	 */
    Log::Log(string newName, string newFilename)
    {
        name = newName;
        filename = newFilename;
        log("Init creation of logfile " + filename);
    }

    /**
     * @brief Log a message with a timestamp and the given flag.
     * @param message The message to be logged.
     * @param flag This message's flag.
     */
    void Log::log(string message, logFlag flag)
    {
        //Output time for logging purposes
        time_t rawTime;
        tm* timeInfo;
        time(&rawTime);
        timeInfo = localtime(&rawTime);

        string flagText;
        switch (flag)
        {
        case error:
            flagText = "[ERROR]\t\t";
            break;
        case info:
            flagText = "[INFO]\t\t";
            break;
        case resource:
            flagText = "[RESOURCE]\t";
            break;
        case hydsys:
            flagText = "[SYSTEM]\t";
            break;
        default:
            flagText = "[NOFLAG]\t";
            break;
        }

        stringstream logEntry;
        logEntry << "[" << timeInfo->tm_hour << ":" << timeInfo->tm_min << ":" << timeInfo->tm_sec << "]\t";
        logEntry << flagText;
        logEntry << message;
        logBuffer.push_back(logEntry.str());
        if (logBuffer.size() >= MAX_LOGBUFFER_ENTRIES)
            flushBuffer();
    }

    /** Flush the log buffer, outputting all messages to file.
     * Log buffers are used to reduce lag from files constantly being opened and closed, because fstream is wierd.
     */
    void Log::flushBuffer()
    {
        //TODO: Have this operate in a separate thread (for efficiency)
        ofstream file;
        file.open(filename, ofstream::out | ofstream::app);
        for (auto iter = logBuffer.begin(); iter != logBuffer.end(); iter++)
            file << *iter << endl;
        file.close();
    }

    unordered_map<string, Log> Logger::logFiles;

    Logger::Logger()
    {
    }

    /**
     * @brief Logs a message to the log designated log.
     * @param message The message to log.
     * @param flag The flag of the message.
     * @param name The name of the log to log to.
     * @note If the log does not exist, it will be created for you as [name].txt under the root directory.
     */
    void Logger::log(string message, string name, logFlag flag)
    {
    	if (logFiles.count(name) == 0)
    		newLog(name, "/" + name + ".txt");
    	logFiles[name].log(message, flag);
    }

    /**
     * @brief Creates a new log with the given name and filename.
     * @param name The name of the log to create.
     * @param filename The file to write to.
     * @note Don't forget a leading '/' on the filename.
     */
    void Logger::newLog(string name, string filename)
    {
    	if (logFiles.count(name) > 0)
    		return; //Duplicate logfile found
        Log _newLog(name, filename);
        logFiles[name] = _newLog;
    }

    /**
     * @brief Gets a pointer to a log of the given name.
     * @param name The name of the log.
     * @return Pointer to the log object. Returns nullptr if the log does not exist.
     */
    Log* Logger::getLog(string name)
    {
        if (logFiles.count(name) == 0)
        	return nullptr;
        return &logFiles[name];
    }

    /**
     * @brief Flush all log buffers, outputting ALL logs to text.
     */
    void Logger::flushLogBuffers()
    {
    	for (auto iter = logFiles.begin(); iter != logFiles.end(); iter++)
    		iter->second.flushBuffer(); //unordered_map iteration gives you an std::pair object
    }
};
