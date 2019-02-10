#pragma once
#include "BaseRecord.h"

namespace AutoPilot
{
	class TouchInfo
	{
	public:
		TouchInfo() { x = y = 0; }
		TouchInfo(int x, int y) { this->x = x; this->y = y; }
		virtual ~TouchInfo() {}
		
		TouchInfo& operator=(const TouchInfo& info) { x = info.x; y = info.y; return *this; }
		bool operator==(const TouchInfo& info) const { return (x == info.x) && (y == info.y); }
		bool operator!=(const TouchInfo& info) const { return (x != info.x) || (y != info.y); }
		
	public:
		int x;
		int y;
	};
	
	class TouchRecord : public BaseRecord
	{
	public:
		TouchRecord() {}
		TouchRecord(const TouchRecord& record): BaseRecord(record) { setTime(record.getTime());  }
		virtual ~TouchRecord() {}
		
		TouchRecord& operator=(const TouchRecord& record) { setTime(getTime()); touch = record.touch; return *this; }
		bool operator==(const TouchRecord& record) const
		{
			bool ret = ((getTime() == record.getTime()) && (touch.size() == record.touch.size()));
			
			if (ret == true)
			{
				for (int loopIdx = 0; loopIdx < touch.size(); ++loopIdx)
				{
					if (touch[loopIdx] != record.touch[loopIdx])
					{
						ret = false;
						break;
					}
				}
			}
			
			return ret;
		}
		bool operator!=(const TouchRecord& record) const { return !operator==(record); }
		
	public:
		std::vector<TouchInfo> touch;
	};
}
