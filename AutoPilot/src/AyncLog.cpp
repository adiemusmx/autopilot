#include "AyncLog.h"
#include <iostream>
#include <thread>

#if defined(_WIN32)
#include <windows.h>
#elif defined(_LINUX)
#include <unistd.h>
#endif

namespace AutoPilot
{
	class AyncLogManager
	{
	public:
	
		static AyncLogManager* instance()
		{
			static AyncLogManager ins;
			return &ins;
		}
		
		/* Initialize and cleanup */
		void init()
		{
			std::thread th(AyncLogManager::AyncAutoFlushProc);
			th.detach();
		}
		void cleanup()
		{
			m_isExit = true;
			
			while (m_isExit)
			{
				Sleep(100);
			}
		}
		
		/* Attach and detach info */
		void attach(AyncLog* info)
		{
			std::lock_guard<std::mutex> temp(m_mutex);
			
			if (std::find(m_coll.begin(), m_coll.end(), info) == m_coll.end())
			{
				m_coll.push_back(info);
			}
		}
		void detach(AyncLog* info)
		{
			std::lock_guard<std::mutex> temp(m_mutex);
			auto iter = std::find(m_coll.begin(), m_coll.end(), info);
			
			if (iter != m_coll.end())
			{
				m_coll.erase(iter);
			}
		}
		
	private:
	
		/* Internal aync flush proc for multi thread */
		static void AyncAutoFlushProc()
		{
			AyncLogManager* manager = instance();
			
			while (manager->m_isExit == false)
			{
				Time nowTime = Time::current();
				manager->m_mutex.lock();
				
				for (auto item : manager->m_coll)
				{
					if ((nowTime - item->m_lastAutoFlushTime) > item->m_autoFlushInterval)
					{
						item->flush();
						item->m_lastAutoFlushTime = nowTime;
					}
				}
				
				manager->m_mutex.unlock();
				Sleep(100);
			}
			
			manager->m_isExit = false;
		}
		
	private:
	
		std::mutex m_mutex;
		std::vector<AyncLog*> m_coll;
		bool m_isExit = false;
	};
	
	void AyncLogManager_init()
	{
		AutoPilot::AyncLogManager::instance()->init();
	}
	
	void AyncLogManager_cleanup()
	{
		AutoPilot::AyncLogManager::instance()->cleanup();
	}
	
	AyncLog::AyncLog(const std::string& fileName)
	{
		m_fileName = fileName;
	}
	
	AyncLog::~AyncLog()
	{
	}
	
	void AyncLog::init()
	{
		m_lastAutoFlushTime = Time::current();
		m_fileStream.open(generateFileName(), std::ios::ate | std::ios::out);
		
		if (!m_fileStream)
		{
			std::cerr << __FUNCTION__ << " : Create File [" << m_fileName << "] failed." << std::endl;
		}
		
		AyncLogManager::instance()->attach(this);
	}
	
	void AyncLog::cleanup()
	{
		AyncLogManager::instance()->detach(this);
		flush();
		
		if (!m_fileStream)
		{
			m_fileStream.close();
		}
	}
	
	void AyncLog::print(const std::string& log)
	{
		m_bufferMutex.lock();
		m_buffer.push(log);
		m_bufferMutex.unlock();
	}
	
	void AyncLog::flush()
	{
		/* Lock buffer mutex */
		m_bufferMutex.lock();
		
		/* If m_buffer is empty, abort this flush operation */
		if (m_buffer.size() == 0 || !m_fileStream)
		{
			return;
		}
		
		while (!m_buffer.empty())
		{
			m_fileStream << m_buffer.front() << std::endl;
			m_buffer.pop();
			++m_lineNumber;
		}
		
		m_bufferMutex.unlock();
		/* Lock stream mutex */
		m_streamMutex.lock();
		m_fileStream.flush();
		
		if (m_lineNumber > m_maxLineNumber)
		{
			m_fileStream.close();
			m_fileStream.open(generateFileName(), std::ios::ate | std::ios::out);
			
			if (!m_fileStream)
			{
				std::cerr << __FUNCTION__ << " : Create File [" << m_fileName << "] failed." << std::endl;
			}
			
			m_lineNumber = 0;
		}
		
		m_streamMutex.unlock();
	}
	
	std::string AyncLog::generateFileName()
	{
		char fileName[256] = { 0 };
		char suffix[10] = {0};
		_itoa_s(m_fileNameSuffix, suffix, 10);
		
		/* Place the suffix between filename and ext */
		for (int loopIdx = m_fileName.length() - 1; loopIdx > 0; --loopIdx)
		{
			if (m_fileName[loopIdx] == '.')
			{
				strncpy_s(fileName, m_fileName.c_str(), loopIdx);
				strcat_s(fileName, "_");
				strcat_s(fileName, suffix);
				strcat_s(fileName, m_fileName.c_str() + loopIdx);
				break;
			}
		}
		
		++m_fileNameSuffix;
		return fileName;
	}
}