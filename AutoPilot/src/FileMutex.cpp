#include "FileMutex.h"
#include <fstream>
#include <iostream>

#if defined(_WIN32)
#include <io.h>
#include <windows.h>
#elif defined(_LINUX)
#include <sys/io.h>
#include <unistd.h>
#endif

#define FILE_MUTEX_LOCK_FILE_EXT ".lock"

namespace AutoPilot
{
	FileMutex::FileMutex(const std::string& fileName)
	{
		m_fileName = fileName;
	}
	
	void FileMutex::lock()
	{
		while (isLocked())
		{
			Sleep(100);
		}
		
		std::ofstream file;
		file.open(m_fileName + FILE_MUTEX_LOCK_FILE_EXT, std::ios::in);
		
		if (!file)
		{
			std::cerr << __FUNCTION__ << " : Create File [" << m_fileName + FILE_MUTEX_LOCK_FILE_EXT << "] failed." << std::endl;
		}
		
		file.close();
	}
	
	void FileMutex::unlock()
	{
		if (remove((m_fileName + FILE_MUTEX_LOCK_FILE_EXT).c_str()) == -1)
		{
			std::cerr << __FUNCTION__ << " : Remove File [" << m_fileName + FILE_MUTEX_LOCK_FILE_EXT << "] failed." << std::endl;
		}
	}
	
	bool FileMutex::isLocked()
	{
#if defined(_WIN32)
		return _access((m_fileName + FILE_MUTEX_LOCK_FILE_EXT).c_str(), 0) == 0 ? true : false;
#elif defined(_LINUX)
		return _access((m_fileName + FILE_MUTEX_LOCK_FILE_EXT).c_str(), 0) == 0 ? true : false;
#endif
	}
}