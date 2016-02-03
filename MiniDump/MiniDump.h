#ifndef __MINIDUMP_H__
#define __MINIDUMP_H__

#ifdef MINIDUMP_EXPORTS
#define MINIDUMP_DLL extern "C" __declspec(dllexport)
#else
#define MINIDUMP_DLL extern "C" __declspec(dllimport)
#endif

#define MD_OK				1
#define MD_ERR				0

#ifdef _UNICODE
#	define MiniDumpSetMailSender	MiniDumpSetMailSenderW
#	define MiniDumpAddMailReceiver	MiniDumpAddMailReceiverW
#else
#	define MiniDumpSetMailSender	MiniDumpSetMailSenderA
#	define MiniDumpAddMailReceiver	MiniDumpAddMailReceiverA
#endif

//////////////////////////////////////////////////////////////////////////

/*
* 函数介绍： 启动MiniDump
* 输入参数： 无
* 输出参数： 无
* 返回值  ： MD_ERR失败，MD_OK成功或其他错误代码*/
MINIDUMP_DLL int MiniDumpStartUp();

/*
* 函数介绍： 关闭MiniDump，MiniDump启动后，无须释放，
*			 此函数只是关闭MiniDump的功能，需要关闭MiniDump时可调用
* 输入参数： 无
* 输出参数： 无
* 返回值  ： MD_ERR失败，MD_OK成功或其他错误代码*/
MINIDUMP_DLL int MiniDumpShutDown();

/*
* 函数介绍： Unicode版本，设置邮件发送服务器、邮件账号、邮件密码
* 输入参数： pMailServer：邮件服务器，pUserName：账号，pPassWord：密码
* 输出参数： 无
* 返回值  ： MD_ERR失败，MD_OK成功或其他错误代码*/
MINIDUMP_DLL int MiniDumpSetMailSenderW(const wchar_t *pMailServer, 
									    const wchar_t *pUserName, 
									    const wchar_t *pPassWord);
/*
* 函数介绍： ASCII版本，设置邮件发送服务器、邮件账号、邮件密码
* 输入参数： pMailServer：邮件服务器，pUserName：账号，pPassWord：密码
* 输出参数： 无
* 返回值  ： MD_ERR失败，MD_OK成功或其他错误代码*/
MINIDUMP_DLL int MiniDumpSetMailSenderA(const char *pMailServer, 
										const char *pUserName, 
										const char *pPassWord);
/*
* 函数介绍： Unicode版本，添加邮件接收人
* 输入参数： pszMailAddr：邮件接收人地址
* 输出参数： 无 
* 返回值  ： MD_ERR失败，MD_OK成功或其他错误代码*/
MINIDUMP_DLL int MiniDumpAddMailReceiverW(const wchar_t *pszMailAddr);

/*
* 函数介绍： ASCII版本，添加邮件接收人
* 输入参数： pszMailAddr：邮件接收人地址
* 输出参数： 无
* 返回值  ： MD_ERR失败，MD_OK成功或其他错误代码*/
MINIDUMP_DLL int MiniDumpAddMailReceiverA(const char *pszMailAddr);

#endif
