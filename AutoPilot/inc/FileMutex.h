#pragma once

#include <mutex>
#include <string>

namespace AutoPilot
{
	/* Use file system to lock file resources */
	class FileMutex
	{
	public:
	
		/* Constructor and destructor */
		FileMutex(const std::string& fileName);
		virtual ~FileMutex() {}
		
		/* Lock & unlock file */
		void lock();
		void unlock();
		
		/* Whether file is locked */
		bool isLocked();
		
	private:
		FileMutex(const FileMutex&) {}
		FileMutex& operator=(const FileMutex&) {}
		
		std::string m_fileName;
	};
}