// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

//XP���
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")


#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <tchar.h>
#include <atlbase.h>
//#include <atlapp.h>
#include <atlwin.h>
//#include <atlframe.h>
//#include <atlddx.h>
//#include <atlctrls.h>
// 
// extern CAppModule _Module;
// 
#include "MailSend.h"
extern CMailSend   _MailSend;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
