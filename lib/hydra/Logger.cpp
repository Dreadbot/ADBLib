#include "Logger.h"

namespace Hydra
{
	/**
	 * @brief Constructor for log.
	 * @param newName the name of the log as referred to when grabbed by Logger.
	 * @param newFileName The filename for the log file. Do NOT append any file extension, it does it for you.
	 * @note If on a linux-based system (such as a roboRIO), you need to add "/" to the beginning of the filename.
	 */
    Log::Log(string newName, string newFilename)
    {
        name = newName;
        filename = newFilename + ".txt";
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
        file.open(filename);
        for (auto iter = logBuffer.begin(); iter != logBuffer.end(); iter++)
            file << *iter << endl;
    }

    Logger* Logger::instance = nullptr;
    Logger* Logger::getInstance()
    {
        if (instance == nullptr)
            instance = new Logger;
        return instance;
    }

    /**
     * @brief Automatically creates its own system log, named "sysLog"
     */
    Logger::Logger()
    {
        newLog("sysLog", "/sysLog");
    }
    Logger::~Logger() {}

    /**
     * @brief Logs a message to the log designated log.
     * @param message The message to log.
     * @param flag The flag of the message.
     * @param name The name of the log to log to.
     */
    void Logger::log(string message, logFlag flag, string name)
    {
        //Find the correct log, then log the message with it
        for (auto iter = logFiles.begin(); iter != logFiles.end(); iter++)
        {
            if (iter->name == name)
            {
                iter->log(message, flag);
                return;
            }
        }
        //At this point, it is confirmed that no log file exists.
        log("Cannot find log " + name + ", creating new one at." + name + ".txt", info);
        Log newLog(name, name);
        newLog.log(message, flag);
        logFiles.push_back(newLog);
    }

    /**
     * @brief Creates a new log with the given name and filename.
     * @param name The name of the log to create.
     * @param filename The file to write to. Do NOT append a file extension, it does it for you.
     */
    void Logger::newLog(string name, string filename)
    {
        //Check for duplicate logs
        for (auto iter = logFiles.begin(); iter != logFiles.end(); iter++)
        {
            if (iter->name == name)
                return; //Duplicate log found.
        }
        Log _newLog(name, filename);
        logFiles.push_back(_newLog);
    }

    /**
     * @brief Gets a pointer to a log of the given name.
     * @param name The name of the log.
     * @return Pointer to the log object. Returns nullptr if the log does not exist.
     */
    Log* Logger::getLog(string name)
    {
        for (auto iter = logFiles.begin(); iter != logFiles.end(); iter++)
        {
            if (iter->name == name)
                return &(*iter);
        }
        return nullptr;
    }

    /**
     * @brief Flush all log buffers, outputting ALL logs to text.
     */
    void Logger::flushLogBuffers()
    {
        for (auto iter = logFiles.begin(); iter != logFiles.end(); iter++)
            iter->flushBuffer();
    }
};
