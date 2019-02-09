#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include "AutoPilotTime.h"

namespace AutoPilot
{
	class BaseRecord
	{
	public:
		virtual std::string toLine() = 0;
		virtual void fromLine(std::string line) = 0;
		
	private:
		Time m_time;
	};
}