#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include "AutoPilotTime.h"

namespace AutoPilot
{
	class BaseRecord
	{
	public:
		BaseRecord() {}
		BaseRecord(const BaseRecord& record) { m_time = record.m_time; }
		virtual ~BaseRecord() {}
		
		/* Separate one line to several strings */
		virtual void analysis(const std::string& line)
		{
			std::string content = m_time.fromLine(line);
			
			/* First character is not '|' */
			if (content.length() == 0 || content[0] != '|')
			{
				std::cerr << __FUNCTION__ << ": Invalid line [" << line << "]" << std::endl;
				return;
			}
			
			/* Proc content */
			if (content.length() > 1)
			{
				fromLine(content.c_str() + 1);
			}
		}
		virtual std::string combine() const {	return m_time.toLine() + "|" + toLine(); }
		
		/* Record Time Format */
		virtual std::string toLine() const { return ""; }
		virtual std::string fromLine(const std::string& line) { return line; }		/* Throw remain content of the string */
		
		/* Time */
		virtual void setTime(const Time& t) { m_time = t; }
		virtual Time getTime() const { return m_time; }
		
	private:
		Time m_time;		/* Time */
	};
}