// CTCPServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CTCPServerDlg.h"
#include "afxdialogex.h"


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

void CTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTCPServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTCPServerDlg 消息处理程序


BOOL CTCPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitSocket();

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
