#pragma once
#include <string>
#include <iostream>

namespace AutoPilot
{
	class Time
	{
	public:
		/* Current Time */
		static Time current();
		
		/* Constructor */
		Time();
		Time(int h, int m, int s, int mill);
		Time(const Time& t);
		
		/* Operator */
		Time& operator=(const Time& t);
		bool operator<(const Time& t) const;
		bool operator>(const Time& t) const;
		bool operator<=(const Time& t) const;
		bool operator>=(const Time& t) const;
		bool operator==(const Time& t) const;
		bool operator!=(const Time& t) const;
		long long operator-(const Time& t) const;
		
		/* Total Millisecond */
		long long getMilliseconds()const;
		
		/* Record Time Format */
		virtual std::string toLine();
		virtual void fromLine(std::string line);
		
	public:
		int hour;
		int minute;
		int second;
		int millisecond;
	};
}