#pragma once

#include <windows.h>
#include <imagehlp.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <tchar.h>
using namespace std;
#pragma comment(lib, "dbghelp.lib")

#ifndef MdString
#	ifdef _UNICODE
		typedef  wstring MdString;
#	else
		typedef  string MdString;
#	endif
#endif

inline void _Format(TCHAR *pszString, const TCHAR *pszFormat,...)
{
	va_list   arglist;
	va_start(arglist, pszFormat);

#ifdef _UNICODE
	wvsprintf(pszString, pszFormat, arglist);
#else
	vsprintf(pszString, pszFormat, arglist);
#endif

	va_end(arglist);
}

inline wchar_t* _A2W(const char *pSrc, int nSrcLen, int &nDstLen)
{
	nDstLen = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLen, NULL, 0);

	wchar_t *pDst = new wchar_t[nDstLen+1];
	memset(pDst, 0, (nDstLen+1)*2);
	MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLen, pDst, nDstLen);
	nDstLen += 1;
	return pDst;
}

inline char *_W2A(const wchar_t *pSrc, int nSrcLen, int &nDstLen)
{
	nDstLen = WideCharToMultiByte(CP_ACP, 0, pSrc, nSrcLen, NULL, 0, NULL, NULL);

	char *pDst = new char[nDstLen+1];
	memset(pDst, 0, (nDstLen+1));
	WideCharToMultiByte(CP_ACP, 0, pSrc, nSrcLen, pDst, nDstLen, NULL, NULL);
	nDstLen += 1;
	return pDst;
}

inline TCHAR* _A2T(const char *pSrc, int nSrcLen, int &nDstLen)
{
#ifdef _UNICODE
	return _A2W(pSrc, nSrcLen, nDstLen);
#else
	TCHAR *pDst = new TCHAR[nSrcLen+1];
	memset(pDst, 0, (nSrcLen+1));
	memcpy(pDst, pSrc, nSrcLen);
	nDstLen = nSrcLen + 1;
	return pDst;
#endif

	return NULL;
}

inline char *_T2A(const TCHAR *pSrc, int nSrcLen, int &nDstLen)
{
#ifdef _UNICODE
	return _W2A(pSrc, nSrcLen, nDstLen);
#else
	char *pDst = new char[nSrcLen+1];
	memset(pDst, 0, (nSrcLen+1));
	memcpy(pDst, pSrc, nSrcLen);
	nDstLen = nSrcLen+1;
	return pDst;
#endif

	return NULL;
}


inline MdString GetCurrentProcessName()
{
	TCHAR czName[MAX_PATH] = {0};
	GetModuleFileName(NULL, czName, MAX_PATH);

	MdString strName = czName;
	int nIndex = strName.find_last_of(_T("\\"));
	if (nIndex != MdString::npos)
	{
		strName = strName.substr(nIndex+1);
		return strName;
	}

	nIndex = strName.find_last_of(_T("/"));
	if (nIndex != MdString::npos)
	{
		strName = strName.substr(nIndex+1);
		return strName;
	}

	return _T("");
}

typedef void (CALLBACK *pfnOutputException)(const TCHAR *pException, const TCHAR *pDmpFilePath);
typedef pfnOutputException PFN_OUTPUT_EXCEPTION;

static LPTOP_LEVEL_EXCEPTION_FILTER g_pfnLastFilter = NULL;
static PFN_OUTPUT_EXCEPTION g_pfnOutputException = NULL;

class CMiniDmp
{
private:
	CMiniDmp() {}

public:
	virtual ~CMiniDmp(){}
	static void Startup() 
	{
		g_pfnLastFilter = SetUnhandledExceptionFilter(DoUnhandledExceptionFilter);
	}

	static void ShutDown()
	{
		SetUnhandledExceptionFilter(g_pfnLastFilter);
	}

	static void SetOutputException(PFN_OUTPUT_EXCEPTION pfnOutputException)
	{
		g_pfnOutputException = pfnOutputException;
	}

private:
	static MdString GetCurrentExePath()
	{
		TCHAR czName[MAX_PATH] = {0};
		GetModuleFileName(NULL, czName, MAX_PATH);

		MdString strName = czName;
		int nIndex = strName.find_last_of(_T("\\"));
		if (nIndex != MdString::npos)
		{
			strName = strName.substr(0, nIndex);
			return strName;
		}

		nIndex = strName.find_last_of(_T("/"));
		if (nIndex != MdString::npos)
		{
			strName = strName.substr(0, nIndex);
			return strName;
		}

		return _T("");
	}

	static BOOL IsDataSectionNeeded(const WCHAR* pModuleName)
	{
		if(pModuleName == 0)
			return FALSE;

		WCHAR szFileName[_MAX_FNAME] = L"";
		_wsplitpath_s(pModuleName, NULL, 0, NULL, 0, szFileName, _MAX_FNAME, NULL, 0);

		if(_wcsicmp(szFileName, L"ntdll") == 0)
			return TRUE;

		return FALSE; 
	}

	static BOOL CALLBACK MiniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput)
	{
		if(pInput == 0 || pOutput == 0)
			return FALSE;

		switch(pInput->CallbackType)
		{
		case ModuleCallback: 
			if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg) 
			//if( !(pOutput->ModuleWriteFlags & ModuleReferencedByMemory) ) 
				if(!IsDataSectionNeeded(pInput->Module.FullPath)) 
					pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg); 
					//pOutput->ModuleWriteFlags &= (~ModuleWriteModule);
		case IncludeModuleCallback:
		case IncludeThreadCallback:
		case ThreadCallback:
		case ThreadExCallback:
			return TRUE;
		default:
			break;
		}

		return FALSE;
	}

	static void CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR strFileName)
	{
		HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,
			0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
		{
			MINIDUMP_EXCEPTION_INFORMATION mdei;
			mdei.ThreadId           = GetCurrentThreadId();
			mdei.ExceptionPointers  = pep;
			mdei.ClientPointers     = FALSE;

			MINIDUMP_CALLBACK_INFORMATION mci;
			mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
			mci.CallbackParam       = 0;

// 			MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory | 
// 				MiniDumpWithDataSegs | 
// 				MiniDumpWithHandleData |
// 				MiniDumpWithFullMemoryInfo | 
// 				MiniDumpWithThreadInfo | 
// 				MiniDumpWithUnloadedModules
// 				);
			MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(
				MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory);
			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
				hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci);

			CloseHandle(hFile); 
		}
	}

	static LONG WINAPI DoUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
	{
		int nProcessID = GetCurrentProcessId();
		int nThreadID = GetCurrentThreadId();

		TCHAR szName[64] = {0};
		_Format(szName, _T("\\Exception_%d_%d.dmp"), nProcessID, nThreadID);

		MdString strPath = GetCurrentExePath();
		strPath += szName;

		CreateMiniDump(pExceptionInfo, strPath.c_str());

		TCHAR szMsg[512] = {0};
		MdString strProcName = GetCurrentProcessName();
		_Format(szMsg,
			_T("%s发生未知错误，错误信息：\r\n    Thread ID:%d\r\n    ExceptionCode:%d\r\n    ExceptionFlags:%d\r\n    ExceptionAddress:%d\r\n    NumberParameters:%d\r\n    错误报告路径:%s"),
			strProcName.c_str(),nThreadID, pExceptionInfo->ExceptionRecord->ExceptionCode,
			pExceptionInfo->ExceptionRecord->ExceptionFlags,
			pExceptionInfo->ExceptionRecord->ExceptionAddress,
			pExceptionInfo->ExceptionRecord->NumberParameters,
			strPath.c_str());

		if(g_pfnOutputException)
			g_pfnOutputException(szMsg, strPath.c_str());

		exit(pExceptionInfo->ExceptionRecord->ExceptionCode);
		return EXCEPTION_EXECUTE_HANDLER;  // 程序停止运行
	}
};