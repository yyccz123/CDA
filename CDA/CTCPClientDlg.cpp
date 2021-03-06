// CTCPClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CTCPClientDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <vector>

using namespace std;

//Macro Definition
#define WM_USER_MSG_ONRECEIVEBUFFER		(WM_USER + 1)
#define WM_USER_MSG_ONSERVERDISCONNECT	(WM_USER + 2)

// Variable Definition
CTCPClientDlg* g_pTCPClientDlg = nullptr;

// CTCPClientDlg 对话框

IMPLEMENT_DYNAMIC(CTCPClientDlg, CDialogEx)

CTCPClientDlg::CTCPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TCPCLIENT, pParent)
{
	ConstructionExtra();
}

CTCPClientDlg::~CTCPClientDlg()
{
	m_Brush.DeleteObject();
}

// CTCPClientDlg 构造函数
void CTCPClientDlg::ConstructionExtra()
{
	m_pSocket = NULL;

	m_hConnect = NULL;
	m_bConnect = false;
	m_bExit = false;
	m_bShareInfo = false;
	g_pTCPClientDlg = this;

	m_uSendCount = 0;
	m_uRecvCount = 0;
	memset(m_chSendBuf, 0, sizeof(m_chSendBuf));
	memset(m_chRecvBuf, 0, sizeof(m_chRecvBuf));

	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

// CTCPClientDlg 窗口初始化
void CTCPClientDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

// CTCPClientDlg 控件初始化
void CTCPClientDlg::InitControls()
{
	BYTE byHostIP[4] = { 0 };

	// 设置远程IP地址
	CCSocketBase::SetLocalIP(NULL, NULL);	// 本机远程IP地址
	CCSocketBase::SetLocalPort(0);			// 本机远程端口
	CCSocketBase::GetLocalIPAddr();			// 获取本机IP地址
	BreakIpAddress(CCSocketBase::GetLocalIP(), byHostIP);	// 拆分本机IP地址
	m_cRemoteIPAdc.SetAddress(byHostIP[0], byHostIP[1], byHostIP[2], byHostIP[3]);	// 设置本机IP地址

	// 设置远程端口号
	m_cRemotePortEdt.SetWindowTextW(_T("6000"));	// 默认远程端口号

	// 关闭发送按钮使能
	m_cSendBtn.EnableWindow(FALSE);

}

// CTCPClientDlg 窗口重绘
void CTCPClientDlg::RePaintWindow(CDC & dc)
{
	CDC MemDC;
	CBitmap MemBitmap;
	CRect MemRect;

	GetClientRect(&MemRect);

	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, MemRect.Width(), MemRect.Height());

	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(&MemRect, RGB(255, 255, 255));

	dc.BitBlt(m_cWindowRect.left, m_cWindowRect.top, m_cWindowRect.Width(), m_cWindowRect.Height(), &MemDC, MemRect.left, MemRect.top, SRCCOPY);

	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

// CTCPClientDlg 初始化Socket
void CTCPClientDlg::InitSocket()
{
	CCSocketBase::CCSocketBaseLibInit();	// 初始化Socket库
}

// CTCPClientDlg 释放Socket
void CTCPClientDlg::ReleaseSocket()
{
	CCSocketBase::CCSocketBaseLibRelease();	// 释放Socket库
}

// CTCPClientDlg 开启连接接收线程
void CTCPClientDlg::StartConnectThread()
{
	unsigned int uThreadID = 0;
	m_hConnect = (HANDLE)::_beginthreadex(NULL, 0, (_beginthreadex_proc_type)OnHandleConnectThread, this, 0, &uThreadID);
}

// CTCPClientDlg 开启处理接收线程
unsigned int CALLBACK CTCPClientDlg::OnHandleConnectThread(LPVOID lpParameters)
{
	while (true)
	{
		char chRecvBuf[SOB_TCP_RECV_BUFFER] = { 0 };
		UINT uRecvCount = 0;
		int nRet = 0;

		g_pTCPClientDlg->m_csThreadSafe.Enter();
		if (g_pTCPClientDlg->m_bExit)
		{
			g_pTCPClientDlg->m_csThreadSafe.Leave();
			break;
		}
		g_pTCPClientDlg->m_csThreadSafe.Leave();

		while (true)
		{
			g_pTCPClientDlg->m_csThreadSafe.Enter();
			if (!g_pTCPClientDlg->m_bShareInfo)
			{
				g_pTCPClientDlg->m_bShareInfo = true;
				g_pTCPClientDlg->m_csThreadSafe.Leave();
				break;
			}
			g_pTCPClientDlg->m_csThreadSafe.Leave();
			Sleep(10);
		}

		nRet = g_pTCPClientDlg->m_pSocket->CCSocketBaseRecvOnce(chRecvBuf, SOB_TCP_RECV_BUFFER, uRecvCount);
		if (nRet == SOB_RET_OK)
		{
			if (strcmp("", chRecvBuf))
			{
				g_pTCPClientDlg->m_uRecvCount = uRecvCount;
				memset(g_pTCPClientDlg->m_chRecvBuf, 0, sizeof(g_pTCPClientDlg->m_chRecvBuf));
				memcpy_s(g_pTCPClientDlg->m_chRecvBuf, sizeof(g_pTCPClientDlg->m_chRecvBuf), chRecvBuf, uRecvCount);
				::PostMessageA(g_pTCPClientDlg->GetSafeHwnd(), WM_USER_MSG_ONRECEIVEBUFFER, (WPARAM)0, (LPARAM)0);
			}
			else
			{
				g_pTCPClientDlg->m_bShareInfo = false;
			}
		}
		else if (nRet == SOB_RET_CLOSE)
		{
			g_pTCPClientDlg->m_bShareInfo = false;

			// 关闭Socket通信
			g_pTCPClientDlg->m_pSocket->CCSocketBaseDisConnect();

			// 发送服务器关闭消息
			::PostMessageA(g_pTCPClientDlg->GetSafeHwnd(), WM_USER_MSG_ONSERVERDISCONNECT, (WPARAM)0, (LPARAM)0);

			break;
		}
		else
		{
			g_pTCPClientDlg->m_bShareInfo = false;
		}

		Sleep(10);
	}

	return 0;
}

// CTCPClientDlg 拆分IP地址
void CTCPClientDlg::BreakIpAddress(const char * pArr, BYTE byArr[4])
{
	char* p = NULL;
	char* pNext = NULL;
	char* pStr = NULL;
	int nSize = 0;
	vector<string> vecStr;

	nSize = strlen(pArr) + 1;
	pStr = new char[nSize];
	memset(pStr, 0, nSize);
	strcpy_s(pStr, nSize, pArr);

	vecStr.clear();

	p = strtok_s(pStr, ".", &pNext);
	while (p != NULL)
	{
		vecStr.push_back(p);
		p = strtok_s(NULL, ".", &pNext);
	}

	for (auto i = 0; i < 4; ++i)
	{
		byArr[i] = atoi(vecStr[i].c_str());
	}

	delete[] pStr;
	pStr = NULL;
}

// CTCPClientDlg 检查IP地址
bool CTCPClientDlg::CheckIpAddress(const char * pArr)
{
	char* p = NULL;
	char* pNext = NULL;
	char* pStr = NULL;
	int nSize = 0;
	vector<string> vecStr;
	bool bRet = true;

	nSize = strlen(pArr) + 1;
	pStr = new char[nSize];
	memset(pStr, 0, nSize);
	strcpy_s(pStr, nSize, pArr);

	vecStr.clear();

	p = strtok_s(pStr, ".", &pNext);
	while (p != NULL)
	{
		vecStr.push_back(p);
		p = strtok_s(NULL, ".", &pNext);
	}

	for (auto iter = vecStr.begin(); iter != vecStr.end(); ++iter)
	{
		if (!strcmp("", iter->c_str()) || atoi(iter->c_str()) < 0 || atoi(iter->c_str()) > 255)
		{
			bRet = false;
			break;
		}

	}

	delete[] pStr;
	pStr = NULL;

	return bRet;
}

// CTCPClientDlg 拆分空格字符串
void CTCPClientDlg::BreakSpace(const unsigned char * pStr, vector<string>& vecStr)
{
	unsigned char* pString = const_cast<unsigned char*>(pStr);
	char* pNewString = nullptr;
	int nSize = 0;
	char* pTemp = nullptr;
	char* pArr = nullptr;

	nSize = strlen((char*)pString) + 1;
	pNewString = new char[nSize];
	memset(pNewString, 0, nSize);
	memcpy_s(pNewString, nSize, pString, strlen((char*)pString));

	vecStr.clear();

	pTemp = strtok_s(pNewString, " ", &pArr);
	while (pTemp)
	{
		vecStr.push_back(pTemp);
		pTemp = strtok_s(NULL, " ", &pArr);
	}

	delete[] pNewString;
	pNewString = nullptr;
}

// CTCPClientDlg 16进制转换
void CTCPClientDlg::Convert2Hex(vector<string>& vecStr, unsigned char * pStr, int nSize)
{
	char* pArr = nullptr;
	int nDelta = 0;

	pArr = new char[nSize];
	memset(pArr, 0, nSize);

	for (auto iter = vecStr.begin(); iter != vecStr.end(); ++iter)
	{
		char* pTemp = const_cast<char*>((*iter).c_str());
		for (size_t i = 0; (i <= (*iter).size()) && ((i + 1) <= (*iter).size()); i += 2, pTemp += 2)
		{
			if (((*pTemp >= '0' && *pTemp <= '9') || (*pTemp >= 'A' && *pTemp <= 'F') || (*pTemp >= 'a' && *pTemp <= 'f'))
				&& ((*(pTemp + 1) >= '0' && *(pTemp + 1) <= '9') || (*(pTemp + 1) >= 'A' && *(pTemp + 1) <= 'F') || (*(pTemp + 1) >= 'a' && *(pTemp + 1) <= 'f')))
			{
				char ch = 0;
				char cl = 0;

				if (*pTemp >= '0' && *pTemp <= '9')
				{
					ch = *pTemp - 48;
				}
				else if (*pTemp >= 'A' && *pTemp <= 'F')
				{
					ch = *pTemp - 55;
				}
				else
				{
					ch = *pTemp - 87;
				}

				if (*(pTemp + 1) >= '0' && *(pTemp + 1) <= '9')
				{
					cl = *(pTemp + 1) - 48;
				}
				else if (*(pTemp + 1) >= 'A' && *(pTemp + 1) <= 'F')
				{
					cl = *(pTemp + 1) - 55;
				}
				else
				{
					cl = *(pTemp + 1) - 87;
				}

				*(pArr + nDelta) = ch * 16 + cl;
				nDelta++;
				if (nDelta >= nSize)
				{
					break;
				}

			}

		}

	}

	m_uSendCount = nDelta;
	memcpy_s(pStr, nSize, pArr, nSize);

	delete[] pArr;
	pArr = nullptr;
}

// CTCPClientDlg 接收服务器Socket发送消息
LRESULT CTCPClientDlg::OnRecvSocketBufferMsg(WPARAM wParam, LPARAM lParam)
{
	CThreadSafe ThreadSafe(m_csThreadSafe.GetCriticalSection());

	CString csRemoteIP = _T("");
	CString csRemotePort = _T("");
	char chServerInfo[MAX_PATH] = { 0 };
	int nLen = 0;

	SYSTEMTIME syTime = { 0 };

	USES_CONVERSION;

	GetLocalTime(&syTime);
	GetDlgItemTextW(IDC_IPADDRESS_NET_CLIENT_IP, csRemoteIP);
	GetDlgItemTextW(IDC_EDIT_NET_CLIENT_PORT, csRemotePort);
	sprintf_s(chServerInfo, "[%s:%s] %02d:%02d:%02d", T2A(csRemoteIP), T2A(csRemotePort), syTime.wHour, syTime.wMinute, syTime.wSecond);

	// 16进制显示数据
	if (TRUE == m_cRecvHexCbx.GetCheck())
	{
		CString csRecvBuff = _T("");
		for (int i = 0; i < m_uRecvCount; ++i)
		{
			CString csTemp = _T("");
			csTemp.Format(_T("%02X "), m_chRecvBuf[i]);
			csRecvBuff += csTemp;
		}

		memset(m_chRecvBuf, 0, sizeof(m_chRecvBuf));
		strcpy_s((char*)m_chRecvBuf, sizeof(m_chRecvBuf), T2A(csRecvBuff));
	}

	// 接收区文本显示
	nLen = m_cRecvEdt.GetWindowTextLengthW();
	if (nLen >= 30000)
	{
		m_cRecvEdt.SetWindowTextW(_T(""));
		nLen = -1;
	}

	m_cRecvEdt.SetSel(nLen, nLen);
	m_cRecvEdt.ReplaceSel(A2T(chServerInfo));
	m_cRecvEdt.ReplaceSel(A2T("\r\n"));
	m_cRecvEdt.ReplaceSel(A2T((char*)m_chRecvBuf));
	m_cRecvEdt.ReplaceSel(A2T("\r\n"));

	m_bShareInfo = false;

	return 0;
}

// CTCPClientDlg 服务器Socket关闭消息
LRESULT CTCPClientDlg::OnServerDisConnectMsg(WPARAM wParam, LPARAM lParam)
{
	int nLen = 0;

	// 接收区文本提示
	nLen = m_cRecvEdt.GetWindowTextLengthW();
	if (nLen >= 30000)
	{
		m_cRecvEdt.SetWindowTextW(_T(""));
		nLen = -1;
	}

	m_cRecvEdt.SetSel(nLen, nLen);
	m_cRecvEdt.ReplaceSel(_T("服务器已关闭\r\n"));
	m_cRecvEdt.ReplaceSel(_T("断开连接成功\r\n"));

	::CloseHandle(m_hConnect);
	m_hConnect = NULL;

	m_bExit = false;

	m_cRemotePortEdt.SetReadOnly(FALSE);
	m_cSendBtn.EnableWindow(FALSE);

	m_bConnect = false;
	m_cConnectBtn.SetWindowTextW(_T("连接"));

	return 0;
}

void CTCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_NET_CLIENT_IP, m_cRemoteIPAdc);
	DDX_Control(pDX, IDC_EDIT_NET_CLIENT_PORT, m_cRemotePortEdt);
	DDX_Control(pDX, IDC_BUTTON_NET_CLIENT_LINK, m_cConnectBtn);
	DDX_Control(pDX, IDC_CHECK_NET_CLIENT_RECV_HEX, m_cRecvHexCbx);
	DDX_Control(pDX, IDC_CHECK_NET_CLIENT_SEND_HEX, m_cSendHexCbx);
	DDX_Control(pDX, IDC_BUTTON_NET_CLIENT_RECV_CLEAR, m_cRecvClearBtn);
	DDX_Control(pDX, IDC_BUTTON_NET_CLIENT_SEND_CLEAR, m_cSendClearBtn);
	DDX_Control(pDX, IDC_EDIT_NET_CLIENT_RECV, m_cRecvEdt);
	DDX_Control(pDX, IDC_EDIT_NET_CLIENT_SEND, m_cSendEdt);
	DDX_Control(pDX, IDC_BUTTON_NET_CLIENT_SEND, m_cSendBtn);
}


BEGIN_MESSAGE_MAP(CTCPClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_NET_CLIENT_LINK, &CTCPClientDlg::OnBnClickedButtonNetClientLink)
	ON_BN_CLICKED(IDC_BUTTON_NET_CLIENT_RECV_CLEAR, &CTCPClientDlg::OnBnClickedButtonNetClientRecvClear)
	ON_BN_CLICKED(IDC_BUTTON_NET_CLIENT_SEND_CLEAR, &CTCPClientDlg::OnBnClickedButtonNetClientSendClear)
	ON_BN_CLICKED(IDC_BUTTON_NET_CLIENT_SEND, &CTCPClientDlg::OnBnClickedButtonNetClientSend)
	ON_MESSAGE(WM_USER_MSG_ONRECEIVEBUFFER, &CTCPClientDlg::OnRecvSocketBufferMsg)
	ON_MESSAGE(WM_USER_MSG_ONSERVERDISCONNECT, &CTCPClientDlg::OnServerDisConnectMsg)
END_MESSAGE_MAP()


// CTCPClientDlg 消息处理程序


BOOL CTCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitSocket();
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTCPClientDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}


HBRUSH CTCPClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		return m_Brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CTCPClientDlg::OnClose()
{
	SAFE_DELETE(m_pSocket);
	ReleaseSocket();
	CDialogEx::OnClose();
}

// CTCPClientDlg 连接
void CTCPClientDlg::OnBnClickedButtonNetClientLink()
{
	bool bRet = false;
	CString csRemoteIP = _T("");
	CString csRemotePort = _T("");
	int nRemotePort = 0;
	BYTE bRemoteIP[4] = { 0 };

	USES_CONVERSION;

	if (!m_bConnect)
	{
		// CSocketBase实例是否存在
		if (m_pSocket == NULL)
		{
			m_pSocket = new CCSocketBase();
		}

		// 获取远程连接IP地址
		GetDlgItemTextW(IDC_IPADDRESS_NET_CLIENT_IP, csRemoteIP);

		// 远程IP地址是否为空
		if (!CheckIpAddress(T2A(csRemoteIP)))
		{
			MessageBox(_T("服务器IP地址不合法!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}

		// 获取远程连接端口号
		m_cRemotePortEdt.GetWindowTextW(csRemotePort);

		nRemotePort = atoi(T2A(csRemotePort));
		if (nRemotePort < 0 || nRemotePort > 65535)
		{
			MessageBox(_T("服务器端口号不合法!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}

		int nLen = 0;

		// 接收区文本提示
		nLen = m_cRecvEdt.GetWindowTextLengthW();
		if (nLen >= 30000)
		{
			m_cRecvEdt.SetWindowTextW(_T(""));
			nLen = -1;
		}

		bRet = m_pSocket->CCSocketBaseConnect(T2A(csRemoteIP), nRemotePort);
		if (!bRet)
		{
			m_cRecvEdt.SetSel(nLen, nLen);
			m_cRecvEdt.ReplaceSel(_T("连接服务器失败\r\n")); 
			return;
		}

		m_cRecvEdt.SetSel(nLen, nLen);
		m_cRecvEdt.ReplaceSel(_T("连接服务器成功\r\n"));

		StartConnectThread();

		m_cRemotePortEdt.SetReadOnly(TRUE);
		m_cSendBtn.EnableWindow(TRUE);

		m_bConnect = true;
		m_cConnectBtn.SetWindowTextW(_T("断开"));
	}
	else
	{
		m_pSocket->CCSocketBaseDisConnect();

		int nLen = 0;

		// 接收区文本提示
		nLen = m_cRecvEdt.GetWindowTextLengthW();
		if (nLen >= 30000)
		{
			m_cRecvEdt.SetWindowTextW(_T(""));
			nLen = -1;
		}

		m_cRecvEdt.SetSel(nLen, nLen);
		m_cRecvEdt.ReplaceSel(_T("断开连接成功\r\n"));

		m_bExit = true;
		::WaitForSingleObject(m_hConnect, INFINITE);
		::CloseHandle(m_hConnect);
		m_hConnect = NULL;

		m_bExit = false;

		m_cRemotePortEdt.SetReadOnly(FALSE);
		m_cSendBtn.EnableWindow(FALSE);

		m_bConnect = false;
		m_cConnectBtn.SetWindowTextW(_T("连接"));
	}

}

// CTCPClientDlg 清空接收区
void CTCPClientDlg::OnBnClickedButtonNetClientRecvClear()
{
	m_cRecvEdt.SetWindowTextW(_T(""));
}

// CTCPClientDlg 清空发送区
void CTCPClientDlg::OnBnClickedButtonNetClientSendClear()
{
	m_cSendEdt.SetWindowTextW(_T(""));
}

// CTCPClientDlg 发送
void CTCPClientDlg::OnBnClickedButtonNetClientSend()
{
	unsigned char chSendBuf[SOB_TCP_SEND_BUFFER] = { 0 };
	int nRet = 0;

	USES_CONVERSION;

	// 获取准备发送的内容
	CString csSendBuf;

	m_cSendEdt.GetWindowTextW(csSendBuf);
	memset(chSendBuf, 0, sizeof(chSendBuf));
	strcpy_s((char*)chSendBuf, sizeof(chSendBuf), T2A(csSendBuf));

	// 16进制发送数据
	if (TRUE == m_cSendHexCbx.GetCheck())
	{
		vector<string> vecSendBuf;

		BreakSpace(chSendBuf, vecSendBuf);
		memset(chSendBuf, 0, sizeof(chSendBuf));
		Convert2Hex(vecSendBuf, chSendBuf, sizeof(chSendBuf));
	}

	// 消息区显示发送的内容
	char chServerInfo[MAX_PATH] = { 0 };
	int nLen = 0;
	SYSTEMTIME syTime = { 0 };

	m_pSocket->GetLocalIPPort();

	GetLocalTime(&syTime);
	sprintf_s(chServerInfo, "[%s:%d] %02d:%02d:%02d", CCSocketBase::GetLocalIP(), CCSocketBase::GetLocalPort(), syTime.wHour, syTime.wMinute, syTime.wSecond);

	// 接收区文本显示
	nLen = m_cRecvEdt.GetWindowTextLengthW();
	if (nLen >= 30000)
	{
		m_cRecvEdt.SetWindowTextW(_T(""));
		nLen = -1;
	}

	m_cRecvEdt.SetSel(nLen, nLen);
	m_cRecvEdt.ReplaceSel(A2T(chServerInfo));
	m_cRecvEdt.ReplaceSel(A2T("\r\n"));
	m_cRecvEdt.ReplaceSel(A2T((char*)chSendBuf));
	m_cRecvEdt.ReplaceSel(A2T("\r\n"));

	// 16进制发送数据
	if (TRUE == m_cSendHexCbx.GetCheck())
	{
		nRet = m_pSocket->CCSocketBaseSendBuffer((char*)chSendBuf, m_uSendCount);
		if (nRet == SOB_RET_OK)
		{
			// 发送消息成功响应...
		}
	}
	else
	{
		nRet = m_pSocket->CCSocketBaseSendOnce((char*)chSendBuf, m_uSendCount);
		if (nRet == SOB_RET_OK)
		{
			// 发送消息成功响应...
		}
	}
	
}
