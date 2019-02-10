#include "AutoPilotTime.h"
#include <regex>
#include <windows.h>

namespace AutoPilot
{
#define TIME_STAMP_LENGTH 13

	Time::Time()
	{
		hour = minute = second = millisecond = 0;
	}
	
	Time::Time(int h, int m, int s, int mill)
	{
		hour = h;
		minute = m;
		second = s;
		millisecond = mill;
	}
	
	Time::Time(const Time& t)
	{
		hour = t.hour;
		minute = t.minute;
		second = t.second;
		millisecond = t.millisecond;
	}
	
	std::string Time::toLine() const
	{
		char time[TIME_STAMP_LENGTH] = { 0 };
		sprintf_s(time, "%02d:%02d:%02d.%03d", hour, minute, second, millisecond);
		return time;
	}
	
	std::string Time::fromLine(const std::string& line)
	{
		std::regex reg("(\\d{2}):(\\d{2}):(\\d{2}).(\\d{3})(.*)");
		std::smatch m;
		
		if (std::regex_match(line, m, reg))
		{
			hour = atoi(m.str(1).data());
			minute = atoi(m.str(2).data());
			second = atoi(m.str(3).data());
			millisecond = atoi(m.str(4).data());
			return m.str(5).data();
		}
		else
		{
			std::cerr << __FUNCTION__ << ": Invalid line [" << line << "]" << std::endl;
			return line;
		}
	}
	
	Time& Time::operator=(const Time& t)
	{
		hour = t.hour;
		minute = t.minute;
		second = t.second;
		millisecond = t.millisecond;
		return *this;
	}
	
	bool Time::operator<(const Time& t) const
	{
		return getMilliseconds() < t.getMilliseconds();
	}
	
	bool Time::operator>(const Time& t) const
	{
		return getMilliseconds() > t.getMilliseconds();
	}
	
	bool Time::operator<=(const Time& t) const
	{
		return getMilliseconds() <= t.getMilliseconds();
	}
	
	bool Time::operator>=(const Time& t) const
	{
		return getMilliseconds() >= t.getMilliseconds();
	}
	
	bool Time::operator==(const Time& t) const
	{
		return getMilliseconds() == t.getMilliseconds();
	}
	
	bool Time::operator!=(const Time& t) const
	{
		return getMilliseconds() != t.getMilliseconds();
	}
	
	long long Time::operator-(const Time& t) const
	{
		return getMilliseconds() - t.getMilliseconds();
	}
	
	long long Time::getMilliseconds() const
	{
		long long ret = 0;
		ret += hour * 60 * 60 * 1000;
		ret += minute * 60 * 1000;
		ret += second * 1000;
		ret += millisecond;
		return ret;
	}
	
	Time Time::current()
	{
#if defined(_WIN32)
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		Time ret(sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		return ret;
#elif defined(_LINUX)
		time_t timer;
		struct tm* t;
		timer = time(NULL);
		t = localtime(&timer);
		// Millisecond
		timeval tv;
		gettimeofday(&tv, NULL);
		Time date(t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
#endif
	}
}

