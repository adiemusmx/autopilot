#pragma once

#include <string>

namespace AutoPilot
{
	class AyncCompress
	{
	public:
	
		/* Init & cleanup environment. */
		static void init();
		static void cleanup();
		
		static void compress(const std::string& file);
		static void uncompress(const std::string& file);
		
	private:
		AyncCompress() {}
		AyncCompress(const AyncCompress&) {}
		AyncCompress& operator=(const AyncCompress&) {}
		virtual ~AyncCompress() {}
	};
}