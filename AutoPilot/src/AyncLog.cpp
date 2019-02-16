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
		
		/* Attach and detach info */
		void attach(std::shared_ptr<AyncLog> info)
		{
			auto iter = std::find_if(m_coll.begin(), m_coll.end(), [&info](std::weak_ptr<AyncLog> log) -> bool
			{
				if (!log.expired())
				{
					auto item = log.lock();
					return item->m_tag == info->m_tag;
				}
			});

			if (iter == m_coll.end())
			{
				m_coll.push_back(info);

				if (m_isRunning == false)
				{
					m_isRunning = true;
					std::thread th(AyncLogManager::AyncAutoFlushProc);
					th.detach();
				}
			}
		}

		void detach(std::shared_ptr<AyncLog> info)
		{
			auto iter = std::find_if(m_coll.begin(), m_coll.end(), [&info](std::weak_ptr<AyncLog> log) -> bool
			{
				if (!log.expired())
				{
					auto item = log.lock();
					return item->m_tag == info->m_tag;
				}
				return false;
			});

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

			while (manager->m_coll.size() > 0)
			{
				Time nowTime = Time::current();

				for (auto ptr : manager->m_coll)
				{
					if (!ptr.expired())
					{
						auto item = ptr.lock();
						if ((nowTime - item->m_lastAutoFlushTime) > item->m_autoFlushInterval)
						{
							item->flush();
							item->m_lastAutoFlushTime = nowTime;
						}
					}
					
				}
				
				Sleep(100);
			}
			
			m_isRunning = false;
		}
		
	private:
	
		std::vector<std::weak_ptr<AyncLog>> m_coll;
		static bool m_isRunning;
	};

	bool AyncLogManager::m_isRunning = false;
	
	AyncLog::AyncLog(const std::string& tag, const std::string& fileName)
	{
		m_tag = tag;
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
		
		AyncLogManager::instance()->attach(shared_from_this());
	}
	
	void AyncLog::cleanup()
	{
		AyncLogManager::instance()->detach(shared_from_this());
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
		/* If m_buffer is empty, abort this flush operation */
		if (m_buffer.size() == 0 || !m_fileStream)
		{
			return;
		}
		
		/* Lock buffer mutex */
		m_bufferMutex.lock();

		while (!m_buffer.empty())
		{
			m_fileStream << "[" << m_tag << "]" << m_buffer.front() << std::endl;
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