// CRealCurveDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CRealCurveDlg.h"
#include "afxdialogex.h"


// CRealCurveDlg 对话框

IMPLEMENT_DYNAMIC(CRealCurveDlg, CDialogEx)

CRealCurveDlg::CRealCurveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REALCURVE_DIALOG, pParent)
{
	ConstructionExtra();
}

CRealCurveDlg::~CRealCurveDlg()
{
	m_Brush.DeleteObject();
}

// CRealCurveDlg 构造函数
void CRealCurveDlg::ConstructionExtra()
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

// CRealCurveDlg 初始化窗口形状
void CRealCurveDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

// CRealCurveDlg 初始化窗口控件
void CRealCurveDlg::InitControls()
{
	// 串口初始化
	m_cSerialPort.EnumSerialPort();		// 枚举串口

	// 添加串口
	m_cSerialPortNameCbx.ResetContent();	// 清空Cbx
	for (auto iter = m_cSerialPort.m_mapEnumCOM.begin(); iter != m_cSerialPort.m_mapEnumCOM.end(); ++iter)
	{
		USES_CONVERSION;
		m_cSerialPortNameCbx.InsertString(iter->first, A2T((iter->second).c_str()));
	}

	m_cSerialPortNameCbx.SetCurSel(0);

	// 添加波特率
	m_cSerialPortBaudCbx.ResetContent();	// 清空Cbx
	m_cSerialPortBaudCbx.InsertString(0, _T("9600"));
	m_cSerialPortBaudCbx.InsertString(1, _T("14400"));
	m_cSerialPortBaudCbx.InsertString(2, _T("19200"));
	m_cSerialPortBaudCbx.InsertString(3, _T("38400"));
	m_cSerialPortBaudCbx.InsertString(4, _T("57600"));
	m_cSerialPortBaudCbx.InsertString(5, _T("115200"));
	m_cSerialPortBaudCbx.InsertString(6, _T("128000"));
	m_cSerialPortBaudCbx.InsertString(7, _T("256000"));
	m_cSerialPortBaudCbx.SetCurSel(5);

	// 添加数据位
	m_cSerialPortDataBitCbx.ResetContent();	// 清空Cbx
	m_cSerialPortDataBitCbx.InsertString(0, _T("5"));
	m_cSerialPortDataBitCbx.InsertString(1, _T("6"));
	m_cSerialPortDataBitCbx.InsertString(2, _T("7"));
	m_cSerialPortDataBitCbx.InsertString(3, _T("8"));
	m_cSerialPortDataBitCbx.SetCurSel(3);

	// 添加停止位
	m_cSerialPortStopBitCbx.ResetContent();	// 清空Cbx
	m_cSerialPortStopBitCbx.InsertString(0, _T("1"));
	m_cSerialPortStopBitCbx.InsertString(1, _T("2"));
	m_cSerialPortStopBitCbx.SetCurSel(0);

	// 添加校验位
	m_cSerialPortCheckBitCbx.ResetContent();	// 清空Cbx
	m_cSerialPortCheckBitCbx.InsertString(0, _T("无校验"));
	m_cSerialPortCheckBitCbx.InsertString(1, _T("奇校验"));
	m_cSerialPortCheckBitCbx.InsertString(2, _T("偶校验"));
	m_cSerialPortCheckBitCbx.SetCurSel(0);

	// 辅助功能
	m_cCurve1Cbx.SetCheck(TRUE);
	m_cCurve2Cbx.SetCheck(TRUE);
	m_cCurve3Cbx.SetCheck(TRUE);
	m_cCurve4Cbx.SetCheck(TRUE);
	m_cCurve5Cbx.SetCheck(FALSE);
	m_cCurve6Cbx.SetCheck(FALSE);
	m_cCurve7Cbx.SetCheck(FALSE);
	m_cCurve8Cbx.SetCheck(FALSE);

	// 曲线初始化
	// 禁止控件自动刷新
	m_cChartCtrl.EnableRefresh(false);

	// 创建曲线坐标轴(底部、左侧)
	CChartStandardAxis* pAxisBot = m_cChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartStandardAxis* pAxisLef = m_cChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);

	// 设置坐标轴范围
	pAxisBot->SetAutomatic(true);
	pAxisLef->SetAutomatic(true);

	// 设置坐标轴名称
	m_cChartCtrl.GetLeftAxis()->GetLabel()->SetText(_T("数值"));
	m_cChartCtrl.GetBottomAxis()->GetLabel()->SetText(_T("时间"));

	// 设置曲线标题名称
	m_cChartCtrl.GetTitle()->AddString(_T("串口曲线"));
	m_cChartCtrl.SetEdgeType(EDGE_SUNKEN);

	// 创建曲线
	/*CChartLineSerie* pLineSeries1 = m_cChartCtrl.CreateLineSerie();
	CChartLineSerie* pLineSeries2 = m_cChartCtrl.CreateLineSerie();

	pLineSeries1->SetSeriesOrdering(poNoOrdering);
	pLineSeries2->SetSeriesOrdering(poNoOrdering);

	for (int i = 0; i < 720; ++i)
	{
		double dX1 = 0.0;
		double dX2 = 0.0;
		double dY1 = 0.0;
		double dY2 = 0.0;

		dX1 = dX2 = i / 360.0 * 2 * 3.1415926;
		dY1 = 10 * sin(i / 360.0 * 2 * 3.1415926);
		dY2 = 10 * cos(i / 360.0 * 2 * 3.1415926);

		pLineSeries1->AddPoint(dX1, dY1);
		pLineSeries2->AddPoint(dX2, dY2);
	}*/

	// 开启控件自动刷新
	m_cChartCtrl.EnableRefresh(true);

}

// CRealCurveDlg 重绘窗口
void CRealCurveDlg::RePaintWindow(CDC & dc)
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

// CRealCurveDlg 检查串口设置
bool CRealCurveDlg::SerialPortConfigCheck()
{
	CString csSerialPortName;
	CString csSerialPortBaud;
	CString csSerialPortDataBits;
	CString csSerialPortStopBits;
	CString csSerialPortCheckBits;

	m_cSerialPortNameCbx.GetWindowTextW(csSerialPortName);
	m_cSerialPortBaudCbx.GetWindowTextW(csSerialPortBaud);
	m_cSerialPortDataBitCbx.GetWindowTextW(csSerialPortDataBits);
	m_cSerialPortStopBitCbx.GetWindowTextW(csSerialPortStopBits);
	m_cSerialPortCheckBitCbx.GetWindowTextW(csSerialPortCheckBits);

	if (_T("") == csSerialPortName)
	{
		MessageBoxW(_T("串口名未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortBaud)
	{
		MessageBoxW(_T("串口波特率未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortDataBits)
	{
		MessageBoxW(_T("串口数据位未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortStopBits)
	{
		MessageBoxW(_T("串口停止位未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortCheckBits)
	{
		MessageBoxW(_T("串口校验位未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

void CRealCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_CURVE, m_cChartCtrl);
	DDX_Control(pDX, IDC_COMBO_CURVE_PORT, m_cSerialPortNameCbx);
	DDX_Control(pDX, IDC_COMBO_CURVE_BOAD, m_cSerialPortBaudCbx);
	DDX_Control(pDX, IDC_COMBO_CURVE_DATABIT, m_cSerialPortDataBitCbx);
	DDX_Control(pDX, IDC_COMBO_CURVE_STOPBIT, m_cSerialPortStopBitCbx);
	DDX_Control(pDX, IDC_COMBO_CURVE_CHECKBIT, m_cSerialPortCheckBitCbx);
	DDX_Control(pDX, IDC_BUTTON_CURVE_REFRESH, m_cSerialPortRefreshBtn);
	DDX_Control(pDX, IDC_BUTTON_CURVE_OPEN, m_cSerialPortOpenBtn);
	DDX_Control(pDX, IDC_CHECK_CURVE1, m_cCurve1Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE2, m_cCurve2Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE3, m_cCurve3Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE4, m_cCurve4Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE5, m_cCurve5Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE6, m_cCurve6Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE7, m_cCurve7Cbx);
	DDX_Control(pDX, IDC_CHECK_CURVE8, m_cCurve8Cbx);
	DDX_Control(pDX, IDC_STATIC_CURVE1_VALUE, m_cCurve1Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE2_VALUE, m_cCurve2Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE3_VALUE, m_cCurve3Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE4_VALUE3, m_cCurve4Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE5_VALUE, m_cCurve5Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE6_VALUE, m_cCurve6Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE7_VALUE, m_cCurve7Stic);
	DDX_Control(pDX, IDC_STATIC_CURVE8_VALUE, m_cCurve8Stic);
	DDX_Control(pDX, IDC_BUTTON_CURVE_CLEAR, m_cCurveClearBtn);
	DDX_Control(pDX, IDC_EDIT_CURVE_INFO_MESSAGE, m_cSerialInfoEdt);
	DDX_Control(pDX, IDC_EDIT_CURVE_INFO_PORT, m_cSerialInfoPortEdt);
	DDX_Control(pDX, IDC_EDIT_CURVE_INFO_BAUD, m_cSerialInfoBaudEdt);
	DDX_Control(pDX, IDC_STATIC_CURVE_RECEIVE_ALL, m_cSerialInfoAllRecvStic);
	DDX_Control(pDX, IDC_BUTTON_CURVE_EXPORT_DATA, m_cCurveExportBtn);
}


BEGIN_MESSAGE_MAP(CRealCurveDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CURVE_REFRESH, &CRealCurveDlg::OnBnClickedButtonCurveRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CURVE_OPEN, &CRealCurveDlg::OnBnClickedButtonCurveOpen)
	ON_BN_CLICKED(IDC_BUTTON_CURVE_CLEAR, &CRealCurveDlg::OnBnClickedButtonCurveClear)
	ON_BN_CLICKED(IDC_BUTTON_CURVE_EXPORT_DATA, &CRealCurveDlg::OnBnClickedButtonCurveExportData)
END_MESSAGE_MAP()


// CRealCurveDlg 消息处理程序


BOOL CRealCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CRealCurveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}


HBRUSH CRealCurveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CRealCurveDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CRealCurveDlg::OnBnClickedButtonCurveRefresh()
{
	m_cSerialPort.EnumSerialPort();		// 枚举串口

	// 添加串口
	m_cSerialPortNameCbx.ResetContent();	// 清空Cbx
	for (auto iter = m_cSerialPort.m_mapEnumCOM.begin(); iter != m_cSerialPort.m_mapEnumCOM.end(); ++iter)
	{
		USES_CONVERSION;
		m_cSerialPortNameCbx.InsertString(iter->first, A2T((iter->second).c_str()));
	}

	m_cSerialPortNameCbx.SetCurSel(0);
}


void CRealCurveDlg::OnBnClickedButtonCurveOpen()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CRealCurveDlg::OnBnClickedButtonCurveClear()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CRealCurveDlg::OnBnClickedButtonCurveExportData()
{
	// TODO: 在此添加控件通知处理程序代码
}
