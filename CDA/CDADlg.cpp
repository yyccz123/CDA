
// CDADlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CDADlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCDADlg 对话框



CCDADlg::CCDADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCDADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_cTabMainTbc);
}

BEGIN_MESSAGE_MAP(CCDADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CCDADlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// CCDADlg 消息处理程序

// CCDADlg 构造函数
void CCDADlg::ConstructionExtra()
{

}

// CCDADlg 初始化TabControl控件
void CCDADlg::InitTabControl()
{
	m_cTabMainTbc.InsertItem(0, _T("串口调试"));
	m_cTabMainTbc.InsertItem(1, _T("实时曲线"));
	m_cTabMainTbc.InsertItem(2, _T("网络调试"));
	m_cTabMainTbc.InsertItem(3, _T("数据分析"));
	m_cTabMainTbc.InsertItem(4, _T("帮助"));
	m_cTabMainTbc.InsertItem(5, _T("关于"));
}

// CCDADlg 初始化子窗口
void CCDADlg::InitChildWindow()
{
	CRect Rect;
	int nWidth;
	int nHeight;

	m_cTabMainTbc.GetWindowRect(&Rect);

	nWidth = Rect.right - Rect.left;
	nHeight = Rect.bottom - Rect.top;

	m_cSerialPortDlg.Create(IDD_SERIALPORT_DIALOG, this);
	m_cSerialPortDlg.SetWindowPos(NULL, Rect.left, Rect.top, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	m_cSerialPortDlg.ShowWindow(SW_SHOW);

	m_cRealCurveDlg.Create(IDD_REALCURVE_DIALOG, this);
	m_cRealCurveDlg.SetWindowPos(NULL, Rect.left, Rect.top, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	m_cRealCurveDlg.ShowWindow(SW_HIDE);

	m_cNetWorkDlg.Create(IDD_NETWORK_DIALOG, this);
	m_cNetWorkDlg.SetWindowPos(NULL, Rect.left, Rect.top, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	m_cNetWorkDlg.ShowWindow(SW_HIDE);

	m_cDataAnalysisDlg.Create(IDD_DATAANALYSIS_DIALOG, this);
	m_cDataAnalysisDlg.SetWindowPos(NULL, Rect.left, Rect.top, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	m_cDataAnalysisDlg.ShowWindow(SW_HIDE);

	m_cHelpDlg.Create(IDD_HELP_DIALOG, this);
	m_cHelpDlg.SetWindowPos(NULL, Rect.left, Rect.top, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	m_cHelpDlg.ShowWindow(SW_HIDE);

	m_cAboutExDlg.Create(IDD_ABOUT_DIALOG, this);
	m_cAboutExDlg.SetWindowPos(NULL, Rect.left, Rect.top, nWidth, nHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	m_cAboutExDlg.ShowWindow(SW_HIDE);
}

BOOL CCDADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ConstructionExtra();	// 构造函数
	InitTabControl();		// 初始化TabControl控件
	InitChildWindow();		// 初始化子窗口

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCDADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCDADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCDADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCDADlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCurSel = 0;

	nCurSel = m_cTabMainTbc.GetCurSel();
	switch (nCurSel)
	{
	case 0:
		// 串口调试
		m_cSerialPortDlg.ShowWindow(SW_SHOW);
		m_cRealCurveDlg.ShowWindow(SW_HIDE);
		m_cNetWorkDlg.ShowWindow(SW_HIDE);
		m_cDataAnalysisDlg.ShowWindow(SW_HIDE);
		m_cHelpDlg.ShowWindow(SW_HIDE);
		m_cAboutExDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		// 实时曲线
		m_cSerialPortDlg.ShowWindow(SW_HIDE);
		m_cRealCurveDlg.ShowWindow(SW_SHOW);
		m_cNetWorkDlg.ShowWindow(SW_HIDE);
		m_cDataAnalysisDlg.ShowWindow(SW_HIDE);
		m_cHelpDlg.ShowWindow(SW_HIDE);
		m_cAboutExDlg.ShowWindow(SW_HIDE);
		break;
	case 2:
		// 网络调试
		m_cSerialPortDlg.ShowWindow(SW_HIDE);
		m_cRealCurveDlg.ShowWindow(SW_HIDE);
		m_cNetWorkDlg.ShowWindow(SW_SHOW);
		m_cDataAnalysisDlg.ShowWindow(SW_HIDE);
		m_cHelpDlg.ShowWindow(SW_HIDE);
		m_cAboutExDlg.ShowWindow(SW_HIDE);
		break;
	case 3:
		// 数据分析
		m_cSerialPortDlg.ShowWindow(SW_HIDE);
		m_cRealCurveDlg.ShowWindow(SW_HIDE);
		m_cNetWorkDlg.ShowWindow(SW_HIDE);
		m_cDataAnalysisDlg.ShowWindow(SW_SHOW);
		m_cHelpDlg.ShowWindow(SW_HIDE);
		m_cAboutExDlg.ShowWindow(SW_HIDE);
		break;
	case 4:
		// 帮助
		m_cSerialPortDlg.ShowWindow(SW_HIDE);
		m_cRealCurveDlg.ShowWindow(SW_HIDE);
		m_cNetWorkDlg.ShowWindow(SW_HIDE);
		m_cDataAnalysisDlg.ShowWindow(SW_HIDE);
		m_cHelpDlg.ShowWindow(SW_SHOW);
		m_cAboutExDlg.ShowWindow(SW_HIDE);
		break;
	case 5:
		// 关于
		m_cSerialPortDlg.ShowWindow(SW_HIDE);
		m_cRealCurveDlg.ShowWindow(SW_HIDE);
		m_cNetWorkDlg.ShowWindow(SW_HIDE);
		m_cDataAnalysisDlg.ShowWindow(SW_HIDE);
		m_cHelpDlg.ShowWindow(SW_HIDE);
		m_cAboutExDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	*pResult = 0;
}
