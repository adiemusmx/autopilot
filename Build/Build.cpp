// Build.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "vld.h"
#include <iostream>
#include "AutoPilotTime.h"
#include "BaseRecord.h"
#include "AyncLog.h"
#include <windows.h>

int main()
{
	int lineCount = 900000;
	std::shared_ptr<AutoPilot::AyncLog> log = std::make_shared<AutoPilot::AyncLog>("MAIN", "test.log");
	log->init();
	log->setMaxLineNumber(1000);
	//while (--lineCount != 0)
	{
		//log.print("Hello World! ");
		Sleep(5);
	}
	log->cleanup();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
