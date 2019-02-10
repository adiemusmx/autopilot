#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <fstream>
#include "AutoPilotTime.h"

namespace AutoPilot
{
	void AyncLogManager_init();
	void AyncLogManager_cleanup();
	
	/* Singleton pattern */
	class AyncLog
	{
		friend class AyncLogManager;
	public:
	
		/* Constructor and Destructor */
		AyncLog(const std::string& fileName);
		virtual ~AyncLog();
		
		/* Initialize and cleanup */
		void init();
		void cleanup();
		
		/* Push log to queue. It will be flushed to file later. */
		void print(const std::string& log);
		
		/* Force flush file */
		void flush();
		
		/* Max line number of file */
		void setMaxLineNumber(int lineNumber) { m_maxLineNumber = lineNumber; }
		
		void setAutoFlushInterval(int interval) {}
		
	private:
	
		/* Disable copy constructor */
		AyncLog(const AyncLog&) {}
		AyncLog& operator=(const AyncLog&) {}
		
		/* Generate file name of log */
		std::string generateFileName();
		
		/* File name of the log */
		std::string m_fileName;
		int m_fileNameSuffix = 0;			/* From 1 to n, suffix of the log file */
		
		/* File object */
		std::ofstream m_fileStream;
		
		/* line number */
		int m_lineNumber = 0;
		int m_maxLineNumber = 100000;
		
		/* Buffer of the logs */
		std::queue<std::string> m_buffer;
		
		/* Mutex of file flush */
		std::mutex m_bufferMutex;
		std::mutex m_streamMutex;
		
		/* Auto flush interval (ms) */
		int m_autoFlushInterval = 3000;
		Time m_lastAutoFlushTime;
	};
}
