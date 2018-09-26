// CTCPServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CTCPServerDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <vector>

using namespace std;

// Variable Definition
CTCPServerDlg* g_pTCPServerDlg = nullptr;

// CTCPServerDlg 对话框

IMPLEMENT_DYNAMIC(CTCPServerDlg, CDialogEx)

CTCPServerDlg::CTCPServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TCPSERVER, pParent)
{
	ConstructionExtra();
}

CTCPServerDlg::~CTCPServerDlg()
{
	m_Brush.DeleteObject();
}

// CTCPServerDlg 构造函数
void CTCPServerDlg::ConstructionExtra()
{
	m_hAccept = NULL;
	m_bListen = false;
	m_bListenExit = FALSE;
	g_pTCPServerDlg = this;

	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

// CTCPServerDlg 初始化窗口形状
void CTCPServerDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

// CTCPServerDlg 初始化控件
void CTCPServerDlg::InitControls()
{
	BYTE byHostIP[4] = { 0 };

	// 设置本机IP地址
	m_cSocket.GetHostIP();	// 获取本机IP地址
	BreakIpAddress(m_cSocket.getHostIP(), byHostIP);	// 拆分本机IP地址
	m_cHostIPAdc.SetAddress(byHostIP[0], byHostIP[1], byHostIP[2], byHostIP[3]);	// 设置本机IP地址

	// 设置本机端口号
	m_cHostPortEdt.SetWindowTextW(_T("6000"));	// 默认本机端口号

	// 设置最大连接数
	m_cServerLinkMaxEdt.SetWindowTextW(_T("10"));	// 默认最大连接数10

	// 设置连接方式
	m_cObjectCbx.ResetContent();
	m_cObjectCbx.InsertString(0, _T("广播"));
	m_cObjectCbx.SetCurSel(0);

}

// CTCPServerDlg 窗口重绘
void CTCPServerDlg::RePaintWindow(CDC & dc)
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

// CTCPServerDlg 初始化Socket
void CTCPServerDlg::InitSocket()
{
	m_cSocket.InitSocketLib();	// 初始化Socket库
}

// CTCPServerDlg 释放Socket
void CTCPServerDlg::ReleaseSocket()
{
	m_cSocket.ReleaseSocketLib();	// 释放Socket库
}

// CTCPServerDlg 建立监听接收线程
void CTCPServerDlg::StartListenAndAcceptThread()
{
	unsigned int uThreadID = 0;
	m_hAccept = (HANDLE)::_beginthreadex(NULL, 0, (_beginthreadex_proc_type)OnListenAndAcceptThread, this, 0, &uThreadID);
}

// CTCPServerDlg 监听接收线程
unsigned int CALLBACK CTCPServerDlg::OnListenAndAcceptThread(LPVOID lpParameters)
{
	g_pTCPServerDlg->m_cSocket.StartListenAndAccept(NULL, HandleAcceptCallback, (DWORD)g_pTCPServerDlg, &(g_pTCPServerDlg->m_bListenExit));
	return 0;
}

// CTCPServerDlg 接收回调函数
void CALLBACK CTCPServerDlg::HandleAcceptCallback(SOCKADDR_IN * pRemoteAddr, SOCKET s, DWORD dwUser)
{

}

// CTCPServerDlg 拆分IP地址
void CTCPServerDlg::BreakIpAddress(const char * pArr, BYTE byArr[4])
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

void CTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_NET_SERVER_IP, m_cHostIPAdc);
	DDX_Control(pDX, IDC_EDIT_NET_SERVER_PORT, m_cHostPortEdt);
	DDX_Control(pDX, IDC_EDIT_NET_SERVER_CONNECT_MAX, m_cServerLinkMaxEdt);
	DDX_Control(pDX, IDC_BUTTON_NET_SERVER_LISTEN, m_cListenBtn);
	DDX_Control(pDX, IDC_CHECK_NET_SERVER_RECV_HEX, m_cRecvHexCbx);
	DDX_Control(pDX, IDC_CHECK_NET_SERVER_SEND_HEX, m_cSendHexCbx);
	DDX_Control(pDX, IDC_BUTTON_NET_SERVER_RECV_CLEAR, m_cRecvClearBtn);
	DDX_Control(pDX, IDC_BUTTON_NET_SERVER_SEND_CLEAR, m_cSendClearBtn);
	DDX_Control(pDX, IDC_EDIT_NET_SERVER_RECV, m_cRecvEdt);
	DDX_Control(pDX, IDC_EDIT_NET_SERVER_SEND, m_cSendEdt);
	DDX_Control(pDX, IDC_COMBO_NET_SERVER_CONNECT_STYLE, m_cObjectCbx);
	DDX_Control(pDX, IDC_BUTTON_NET_SERVER_SEND, m_cSendBtn);
}


BEGIN_MESSAGE_MAP(CTCPServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_NET_SERVER_LISTEN, &CTCPServerDlg::OnBnClickedButtonNetServerListen)
	ON_BN_CLICKED(IDC_BUTTON_NET_SERVER_RECV_CLEAR, &CTCPServerDlg::OnBnClickedButtonNetServerRecvClear)
	ON_BN_CLICKED(IDC_BUTTON_NET_SERVER_SEND_CLEAR, &CTCPServerDlg::OnBnClickedButtonNetServerSendClear)
	ON_BN_CLICKED(IDC_BUTTON_NET_SERVER_SEND, &CTCPServerDlg::OnBnClickedButtonNetServerSend)
END_MESSAGE_MAP()


// CTCPServerDlg 消息处理程序


BOOL CTCPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitSocket();
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTCPServerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}


HBRUSH CTCPServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CTCPServerDlg::OnClose()
{
	ReleaseSocket();
	CDialogEx::OnClose();
}

// CTCPServerDlg 监听
void CTCPServerDlg::OnBnClickedButtonNetServerListen()
{
	bool bRet = false;
	CString csHostPort;
	int nHostPort;

	USES_CONVERSION;

	if (!m_bListen)
	{
		m_cHostPortEdt.GetWindowTextW(csHostPort);
		if (csHostPort.IsEmpty())
		{
			MessageBox(_T("端口号不能为空!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}

		nHostPort = atoi(T2A(csHostPort));
		if (nHostPort < 0 || nHostPort > 65535)
		{
			MessageBox(_T("端口号不合法!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}

		bRet = m_cSocket.BindOnPort(nHostPort);
		if (!bRet)
		{
			MessageBox(_T("端口号已被占用!"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}

		StartListenAndAcceptThread();

		m_bListen = true;
		m_cListenBtn.SetWindowTextW(_T("停止"));
	}
	else
	{
		m_bListenExit = TRUE;
		if (NULL != m_hAccept)
		{
			::WaitForSingleObject(m_hAccept, INFINITE);
			::CloseHandle(m_hAccept);
			m_hAccept = NULL;
		}
		m_bListenExit = FALSE;
		m_cSocket.Destroy();

		m_bListen = false;
		m_cListenBtn.SetWindowTextW(_T("监听"));
	}

}

// CTCPServerDlg 清空接收区
void CTCPServerDlg::OnBnClickedButtonNetServerRecvClear()
{
	
}

// CTCPServerDlg 清空发送区
void CTCPServerDlg::OnBnClickedButtonNetServerSendClear()
{
	
}

// CTCPServerDlg 发送
void CTCPServerDlg::OnBnClickedButtonNetServerSend()
{
	
}
