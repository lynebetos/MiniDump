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
* �������ܣ� ����MiniDump
* ��������� ��
* ��������� ��
* ����ֵ  �� MD_ERRʧ�ܣ�MD_OK�ɹ��������������*/
MINIDUMP_DLL int MiniDumpStartUp();

/*
* �������ܣ� �ر�MiniDump��MiniDump�����������ͷţ�
*			 �˺���ֻ�ǹر�MiniDump�Ĺ��ܣ���Ҫ�ر�MiniDumpʱ�ɵ���
* ��������� ��
* ��������� ��
* ����ֵ  �� MD_ERRʧ�ܣ�MD_OK�ɹ��������������*/
MINIDUMP_DLL int MiniDumpShutDown();

/*
* �������ܣ� Unicode�汾�������ʼ����ͷ��������ʼ��˺š��ʼ�����
* ��������� pMailServer���ʼ���������pUserName���˺ţ�pPassWord������
* ��������� ��
* ����ֵ  �� MD_ERRʧ�ܣ�MD_OK�ɹ��������������*/
MINIDUMP_DLL int MiniDumpSetMailSenderW(const wchar_t *pMailServer, 
									    const wchar_t *pUserName, 
									    const wchar_t *pPassWord);
/*
* �������ܣ� ASCII�汾�������ʼ����ͷ��������ʼ��˺š��ʼ�����
* ��������� pMailServer���ʼ���������pUserName���˺ţ�pPassWord������
* ��������� ��
* ����ֵ  �� MD_ERRʧ�ܣ�MD_OK�ɹ��������������*/
MINIDUMP_DLL int MiniDumpSetMailSenderA(const char *pMailServer, 
										const char *pUserName, 
										const char *pPassWord);
/*
* �������ܣ� Unicode�汾������ʼ�������
* ��������� pszMailAddr���ʼ������˵�ַ
* ��������� �� 
* ����ֵ  �� MD_ERRʧ�ܣ�MD_OK�ɹ��������������*/
MINIDUMP_DLL int MiniDumpAddMailReceiverW(const wchar_t *pszMailAddr);

/*
* �������ܣ� ASCII�汾������ʼ�������
* ��������� pszMailAddr���ʼ������˵�ַ
* ��������� ��
* ����ֵ  �� MD_ERRʧ�ܣ�MD_OK�ɹ��������������*/
MINIDUMP_DLL int MiniDumpAddMailReceiverA(const char *pszMailAddr);

#endif
