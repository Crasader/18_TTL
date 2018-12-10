#include "main.h"
#include "Main/AppDelegate.h"
#include "cocos2d.h"

//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
//////////////////////////////////////////////////////////////////////////

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	if (true)
	{
		AllocConsole();										//打开控制台窗口以显示调试信息
		SetConsoleTitleA("Debug Win Output");				//设置标题
		HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);		//获取控制台输出句柄
		INT hCrt = _open_osfhandle((INT)hCon, _O_TEXT);		//转化为C文件描述符
		FILE * hf = _fdopen( hCrt, "w" );					//转化为C文件流
		setvbuf( hf, NULL, _IONBF, 0);						//无缓冲
		*stdout = *hf;										//重定向标准输出	
	}

	UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}