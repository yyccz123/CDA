// CHelpDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CHelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 对话框

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP_DIALOG, pParent)
{
	ConstructionExtra();
}

CHelpDlg::~CHelpDlg()
{
	m_Brush.DeleteObject();
}

// CHelpDlg 构造函数
void CHelpDlg::ConstructionExtra()
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

// CHelpDlg 初始化窗口形状
void CHelpDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

// CHelpDlg 初始化控件
void CHelpDlg::InitControls()
{
	// 帮助项
	m_cHelpContextCbx.ResetContent();

	m_cHelpContextCbx.InsertString(0, _T("串口调试"));
	m_cHelpContextCbx.InsertString(1, _T("实时曲线"));
	m_cHelpContextCbx.InsertString(2, _T("网络调试"));
	m_cHelpContextCbx.InsertString(3, _T("数据分析"));

	m_cHelpContextCbx.SetCurSel(0);

	// 帮助块
	m_cHelpBlockCbx.ResetContent();

	m_cHelpBlockCbx.InsertString(0, _T("应用"));
	m_cHelpBlockCbx.InsertString(1, _T("代码"));

	m_cHelpBlockCbx.SetCurSel(0);

}

// CHelpDlg 重绘窗口
void CHelpDlg::RePaintWindow(CDC & dc)
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

// CHelpDlg 控制台写入一行
void CHelpDlg::ConsoleWriteLine(CString csText)
{
	int nLen = 0;

	// 帮助区文本显示
	nLen = m_cHelpTextEdt.GetWindowTextLengthW();
	if (nLen >= 30000)
	{
		m_cHelpTextEdt.SetWindowTextW(_T(""));
		nLen = -1;
	}

	m_cHelpTextEdt.SetSel(nLen, nLen);
	m_cHelpTextEdt.ReplaceSel(csText);
	m_cHelpTextEdt.ReplaceSel(_T("\r\n"));
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_HELP_CONTEXT, m_cHelpContextCbx);
	DDX_Control(pDX, IDC_COMBO_HELP_BLOCK, m_cHelpBlockCbx);
	DDX_Control(pDX, IDC_EDIT_HELP_TEXT, m_cHelpTextEdt);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_cHelpBtn);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CHelpDlg::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CHelpDlg 消息处理程序


BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHelpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}


HBRUSH CHelpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

// CHelpDlg 查找帮助
void CHelpDlg::OnBnClickedButtonHelp()
{
	int nCurselContext = 0;
	int nCurselBlock = 0;

	nCurselContext = m_cHelpContextCbx.GetCurSel();
	nCurselBlock = m_cHelpBlockCbx.GetCurSel();

	if (nCurselContext == 0)
	{
		// 串口调试
		if (nCurselBlock == 0)
		{
			// 应用
			m_cHelpTextEdt.SetWindowTextW(_T(""));

			ConsoleWriteLine(_T("串口调试帮助:"));
			ConsoleWriteLine(_T("串口调试模块主要用于调试从控制器发送的串口数据, 支持多种协议, 支持多种传输方式."));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("控制器支持:MCU、DSP、FPGA...")); 
			ConsoleWriteLine(_T("协议支持:RS-232、RS-422、RS-485"));
			ConsoleWriteLine(_T("传输方式:串口、蓝牙、2.4GHz射频"));
			ConsoleWriteLine(_T(""));

		}
		else if (nCurselBlock = 1)
		{
			// 代码
			m_cHelpTextEdt.SetWindowTextW(_T(""));

		}

	}
	else if (nCurselContext == 1)
	{
		// 实时曲线
		if (nCurselBlock == 0)
		{
			// 应用
			m_cHelpTextEdt.SetWindowTextW(_T(""));

			ConsoleWriteLine(_T("实时曲线帮助:"));
			ConsoleWriteLine(_T("实时曲线模块主要用于显示从控制器发送的串口数据的曲线显示."));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("串口曲线发送格式:"));
			ConsoleWriteLine(_T("编码格式:0xFF, 0x00, 0x16, 0x08, ...曲线i..., 0xAA, 0x55"));
			ConsoleWriteLine(_T("(8位发送, 先高后低)"));
			ConsoleWriteLine(_T(""));

		}
		else if (nCurselBlock = 1)
		{
			// 代码
			m_cHelpTextEdt.SetWindowTextW(_T(""));

			ConsoleWriteLine(_T("串口曲线发送格式:"));
			ConsoleWriteLine(_T("(格式:0xFF, 0x00, 0x16, 0x08, ...曲线i..., 0xAA, 0x55)"));
			ConsoleWriteLine(_T("(8位发送, 先高后低)"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("signed int SendData_Temp;"));
			ConsoleWriteLine(_T("unsigned char SendData_Temp_High, SendData_Temp_Low;"));
			ConsoleWriteLine(_T(""));
			
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, 0xFF);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, 0x00);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, 0x16);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, 0x08);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线1"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)DAC_Count;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线2"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)giAcc_Z;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线3"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)giGyro_Z;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线4"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)0;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线4"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)0;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线5"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)0;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线6"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)0;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线7"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)0;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("//曲线8"));
			ConsoleWriteLine(_T("SendData_Temp = (signed int)0;"));
			ConsoleWriteLine(_T("SendData_Temp_High = SendData_Temp >> 8;"));
			ConsoleWriteLine(_T("SendData_Temp_Low = SendData_Temp & 0xff;"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_High);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, SendData_Temp_Low);"));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, 0xAA);"));
			ConsoleWriteLine(_T("UART_PutChar(UART_UART4, 0x55);"));
		}

	}
	else if (nCurselContext == 2)
	{
		// 网络调试
		if (nCurselBlock == 0)
		{
			// 应用
			m_cHelpTextEdt.SetWindowTextW(_T(""));

			ConsoleWriteLine(_T("网络调试帮助:"));
			ConsoleWriteLine(_T("网络调试模块主要用于调试局域网内不同设备之间的通信数据."));
			ConsoleWriteLine(_T(""));

			ConsoleWriteLine(_T("网络调试支持:TCP、UDP."));
			ConsoleWriteLine(_T(""));

		}
		else if (nCurselBlock = 1)
		{
			// 代码
			m_cHelpTextEdt.SetWindowTextW(_T(""));

		}

	}
	else if(nCurselContext == 3)
	{
		// 数据分析
		if (nCurselBlock == 0)
		{
			// 应用
			m_cHelpTextEdt.SetWindowTextW(_T(""));

		}
		else if (nCurselBlock = 1)
		{
			// 代码
			m_cHelpTextEdt.SetWindowTextW(_T(""));

		}

	}

}
