// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../MiniDump/MiniDump.h"
#pragma comment(lib, "..//Release\\MiniDump.lib")

int ni = 112;

int _tmain(int argc, _TCHAR* argv[])
{
	MiniDumpStartUp();
	MiniDumpSetMailSender(_T("smtp.chinatransinfo.com"), _T("wangxulin@chinatransinfo.com"), _T("qwer1234"));
	MiniDumpAddMailReceiver(_T("linlin-ring@163.com"));

	char *p = NULL;
	*p = ni;
	return 0;
}

