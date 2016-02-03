// MiniDump.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "MiniDump.h"
#include "minidmp.h"

void CALLBACK OutputException(const TCHAR *pException, const TCHAR *pDmpFilePath)
{
// 	MessageBeep(MB_ICONHAND);
 	_MailSend.SetInfo(pException, pDmpFilePath);
	_MailSend.SendMail();
// 
// 	CMessageLoop theLoop;
// 	_Module.AddMessageLoop(&theLoop);
// 
// 	if(_InfoDlg.Create(NULL) == NULL)
// 	{
// 		ATLTRACE(_T("Main dialog creation failed!\n"));
// 		return;
// 	}
// 
// 	_InfoDlg.ShowWindow(SW_SHOWDEFAULT);
// 
// 	int nRet = theLoop.Run();
// 
// 	_Module.RemoveMessageLoop();
// 	_Module.Term();

// 	::CoUninitialize();
}

MINIDUMP_DLL int MiniDumpStartUp()
{
	CMiniDmp::SetOutputException(OutputException);
	CMiniDmp::Startup();
	return MD_OK;
}

MINIDUMP_DLL int MiniDumpShutDown()
{	
	CMiniDmp::ShutDown();
	return MD_OK;
}

MINIDUMP_DLL int MiniDumpSetMailSenderW(const wchar_t *pMailServer, 
									    const wchar_t *pUserName, 
									    const wchar_t *pPassWord)
{
#ifndef _UNICODE
	int nDstLen = 0;
	char *pServer = _W2A(pMailServer, wcslen(pMailServer), nDstLen);
	char *pUser   = _W2A(pUserName, wcslen(pUserName), nDstLen);
	char *pPwd    = _W2A(pPassWord, wcslen(pPassWord), nDstLen);

	_MailSend.SetMailSender(pServer, pUser, pPwd);

	delete[] pServer;
	delete[] pUser;
	delete[] pPwd;
#else
	_MailSend.SetMailSender(pMailServer, pUserName, pPassWord);
#endif

	return MD_OK;
}

MINIDUMP_DLL int MiniDumpSetMailSenderA(const char *pMailServer, 
										const char *pUserName, 
										const char *pPassWord)
{
#ifdef _UNICODE
	int nDstLen = 0;
	wchar_t *pServer = _A2W(pMailServer, strlen(pMailServer), nDstLen);
	wchar_t *pUser   = _A2W(pUserName, strlen(pUserName), nDstLen);
	wchar_t *pPwd    = _A2W(pPassWord, strlen(pPassWord), nDstLen);

	_MailSend.SetMailSender(pServer, pUser, pPwd);

	delete[] pServer;
	delete[] pUser;
	delete[] pPwd;
#else
	_MailSend.SetMailSender(pMailServer, pUserName, pPassWord);
#endif

	return MD_OK;
}

MINIDUMP_DLL int MiniDumpAddMailReceiverW(const wchar_t *pszMailAddr)
{
#ifndef _UNICODE
	int nDstLen = 0;
	char *pAddr = _W2A(pszMailAddr, wcslen(pszMailAddr), nDstLen);

	_MailSend.AddMailReceiver(pAddr);

	delete[] pAddr;
#else
	_MailSend.AddMailReceiver(pszMailAddr);
#endif

	return MD_OK;
}

MINIDUMP_DLL int MiniDumpAddMailReceiverA(const char *pszMailAddr)
{
#ifdef _UNICODE
	int nDstLen = 0;
	wchar_t *pAddr = _A2W(pszMailAddr, strlen(pszMailAddr), nDstLen);

	_MailSend.AddMailReceiver(pAddr);

	delete[] pAddr;
#else
	_MailSend.AddMailReceiver(pszMailAddr);
#endif

	return MD_OK;
}