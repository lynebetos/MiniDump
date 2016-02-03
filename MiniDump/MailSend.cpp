#include "StdAfx.h"
#include "MailSend.h"

CMailSend::CMailSend()
{

}

CMailSend::~CMailSend()
{

}

void CMailSend::SendMail()
{
	if(m_strMailServer.empty() || m_strMailUser.empty())
		return;

	wofstream ofs("log.txt", ios::out);

	if(!ofs.is_open())
		return;

	char szHost[128] = {0};
	gethostname(szHost, sizeof(szHost));
	hostent *phost = gethostbyname(szHost);

	int nDestLen = 0;
	TCHAR *pszHost = M_A2T(szHost, sizeof(szHost), nDestLen);
	TCHAR *pszIP   = M_A2T(inet_ntoa(*(IN_ADDR*)phost->h_addr_list[0]), 32, nDestLen);

	TCHAR szInfo[512] = {0};
	M_Format(szInfo, _T("From:%s IP:%s\r\n%s"), pszHost, pszIP, m_strInfo.c_str());

	delete[] pszIP;
	delete[] pszHost;

	SMailer::TextPlainContent  textContent(szInfo);
	ofs<<L"textContent"<<endl;

	SMailer::AppOctStrmContent AppContent(m_strFilePath);
	ofs<<m_strFilePath.c_str()<<endl;

	SMailer::MailInfo info;
	info.setSenderName(m_strMailUser);
	info.setSenderAddress(m_strMailUser);
	info.setPriority(SMailer::Priority::normal);
	info.setSubject(_T("Crash Bug"));
	info.addMimeContent(&textContent);
	info.addMimeContent(&AppContent);
	ofs<<L"m_strMailUser = "<<m_strMailUser.c_str()<<endl;

	for(unsigned int i=0; i<m_vecMailReceiver.size(); i++)
		info.addReceiver(m_vecMailReceiver.at(i), m_vecMailReceiver.at(i));

	ofs<<m_vecMailReceiver.at(0).c_str()<<endl;

	try
	{
		ofs<<m_strMailServer.c_str()<<";"<<m_strMailUser.c_str()<<";"<<m_strMailPwd.c_str()<<endl;
		SMailer::MailSender sender(m_strMailServer, m_strMailUser, m_strMailPwd);
		ofs<<L"setMail"<<endl;
		sender.setMail(&SMailer::MailWrapper(&info));
		ofs<<L"sendMail"<<endl;
		sender.sendMail();
		ofs<<L"Send over."<<endl;
	}
	catch (SMailer::MailException& e)
	{
		cout<<e.error();
		ofs<<e.error();
		throw e.error();
	}
	catch (...)
	{
		throw _T("Unkown error");
	}

	ofs.close();

}