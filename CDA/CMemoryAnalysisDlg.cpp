// CMemoryAnalysisDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CMemoryAnalysisDlg.h"
#include "afxdialogex.h"

#define	WM_TIMER_ONGETMEMORYINFO	0

// CMemoryAnalysisDlg 对话框

IMPLEMENT_DYNAMIC(CMemoryAnalysisDlg, CDialogEx)

CMemoryAnalysisDlg::CMemoryAnalysisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MEMERY, pParent)
{
	ConstructionExtra();
}

CMemoryAnalysisDlg::~CMemoryAnalysisDlg()
{
	m_Brush.DeleteObject();
}

void CMemoryAnalysisDlg::ConstructionExtra()
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
	m_bStart = false;
	m_vecMemory.clear();
}

void CMemoryAnalysisDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

void CMemoryAnalysisDlg::InitControls()
{
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
	m_cChartCtrl.GetTitle()->AddString(_T("内存分析"));
	m_cChartCtrl.SetEdgeType(EDGE_SUNKEN);

	// 开启控件自动刷新
	m_cChartCtrl.EnableRefresh(true);
}

void CMemoryAnalysisDlg::RePaintWindow(CDC & dc)
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

void CMemoryAnalysisDlg::CurveAddPoint(double dValue)
{
	static int nVectorSize = 100000;

	if (m_vecMemory.size() < nVectorSize)
	{
		m_vecMemory.push_back(dValue);
	}
	else
	{
		m_vecMemory.erase(m_vecMemory.begin());
		m_vecMemory.push_back(dValue);
	}

}

void CMemoryAnalysisDlg::CurveDraw()
{
	m_cChartCtrl.EnableRefresh(false);

	// 清空图表
	m_cChartCtrl.RemoveAllSeries();

	// 绘制图表
	CChartLineSerie* pCurveSeries1 = m_cChartCtrl.CreateLineSerie();

	pCurveSeries1->SetSeriesOrdering(poNoOrdering);

	// 曲线1
	for (auto iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
	{
		pCurveSeries1->AddPoint(iter - m_vecMemory.begin(), *iter);
	}

	m_cChartCtrl.EnableRefresh(true);
}

void CMemoryAnalysisDlg::CurveExportData()
{
	CString	csFilePathName;
	CString csFileName;

	setlocale(LC_ALL, "chs");
	CFileDialog cFileDialog(false, _T("*.csv"), _T("MemoryCurve"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("*.csv|*.csv||"));
	if (IDOK == cFileDialog.DoModal())
	{
		csFilePathName = cFileDialog.GetPathName();
		csFileName = cFileDialog.GetFileName();

		// 删除已存在文件
		CFileFind findFile;
		BOOL bWorking = findFile.FindFile(csFilePathName);
		if (bWorking)
		{
			CFile::Remove((LPCTSTR)csFilePathName);
		}
		Sleep(100);
		CStdioFile  file(csFilePathName, CFile::modeCreate | CFile::modeReadWrite); // 重新创建
		Sleep(100);

		CString csTip = _T("CDA Memory Analysis Curve Export\n");
		CString csTitle = _T("内存曲线\n");
		file.WriteString(csTip);
		file.WriteString(csTitle);

		for (size_t i = 0; i < m_vecMemory.size(); ++i)
		{
			CString csText = _T("");
			CString csCurve1Text = _T("");

			csCurve1Text.Format(_T("%f\n"), m_vecMemory[i]);

			csText = csCurve1Text;
			file.WriteString(csText);
		}

		file.Close();
	}
}

void CMemoryAnalysisDlg::CurveExportPicture()
{
	CString	csFilePathName;
	CString csFileName;

	setlocale(LC_ALL, "chs");
	CFileDialog cFileDialog(false, _T("*.png"), _T("MemoryPhoto"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("*.png|*.png||"));
	if (IDOK == cFileDialog.DoModal())
	{
		csFilePathName = cFileDialog.GetPathName();
		csFileName = cFileDialog.GetFileName();

		CRect Rect;
		TChartString csFile(csFilePathName);

		m_cChartCtrl.GetClientRect(&Rect);
		m_cChartCtrl.SaveAsImage(csFile, Rect, 32, GUID_NULL);

		MessageBoxW(_T("内存快照保存成功!"), _T("提示"), MB_OK | MB_ICONASTERISK);

	}
}

void CMemoryAnalysisDlg::OnGetProcessMemory()
{
	CString csProcessHanlde = _T("");
	DWORD hProcess = 0;

	m_cProcessHandleEdt.GetWindowTextW(csProcessHanlde);
	if (csProcessHanlde.IsEmpty())
	{
		MessageBoxW(_T("进程句柄不能为空!"), _T(""), MB_OK | MB_ICONWARNING);
		::KillTimer(this->GetSafeHwnd(), WM_TIMER_ONGETMEMORYINFO);
		m_cStartBtn.SetWindowTextW(_T("开启"));
		m_bStart = false;
		return;
	}

	USES_CONVERSION;
	hProcess = strtol(T2A(csProcessHanlde), NULL, 16);

	BOOL bRet = FALSE;
	PROCESS_MEMORY_COUNTERS sProcessInfo = { 0 };

	bRet = GetProcessMemoryInfo((HANDLE)hProcess, (PPROCESS_MEMORY_COUNTERS)(&sProcessInfo), sizeof(sProcessInfo));
	if (FALSE == bRet)
	{
		MessageBoxW(_T("进程内存信息获取失败!"), _T(""), MB_OK | MB_ICONWARNING);
		::KillTimer(this->GetSafeHwnd(), WM_TIMER_ONGETMEMORYINFO);
		m_cStartBtn.SetWindowTextW(_T("开启"));
		m_bStart = false;
		return;
	}

	double dMemoryValue = sProcessInfo.WorkingSetSize * 1.0f / 1024.0f / 1024.0f;
	CurveAddPoint(dMemoryValue);
	CurveDraw();

}

void CMemoryAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_MEMORY, m_cChartCtrl);
	DDX_Control(pDX, IDC_EDIT_MEMORY_PID, m_cProcessIDEdt);
	DDX_Control(pDX, IDC_EDIT_MEMORY_HANDLE, m_cProcessHandleEdt);
	DDX_Control(pDX, IDC_BUTTON_MEMORY_CLEAR, m_cClearBtn);
	DDX_Control(pDX, IDC_BUTTON_MEMORY_START, m_cStartBtn);
	DDX_Control(pDX, IDC_BUTTON_MEMORY_FIND, m_cFindBtn);
	DDX_Control(pDX, IDC_BUTTON_MEMORY_EXPORTDATA, m_cExportDataBtn);
	DDX_Control(pDX, IDC_BUTTON_MEMORY_EXPORTPICTURE, m_cExportPictureBtn);
}


BEGIN_MESSAGE_MAP(CMemoryAnalysisDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_MEMORY_FIND, &CMemoryAnalysisDlg::OnBnClickedButtonMemoryFind)
	ON_BN_CLICKED(IDC_BUTTON_MEMORY_START, &CMemoryAnalysisDlg::OnBnClickedButtonMemoryStart)
	ON_BN_CLICKED(IDC_BUTTON_MEMORY_EXPORTDATA, &CMemoryAnalysisDlg::OnBnClickedButtonMemoryExportdata)
	ON_BN_CLICKED(IDC_BUTTON_MEMORY_EXPORTPICTURE, &CMemoryAnalysisDlg::OnBnClickedButtonMemoryExportpicture)
	ON_BN_CLICKED(IDC_BUTTON_MEMORY_CLEAR, &CMemoryAnalysisDlg::OnBnClickedButtonMemoryClear)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMemoryAnalysisDlg 消息处理程序


BOOL CMemoryAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CMemoryAnalysisDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CMemoryAnalysisDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}


void CMemoryAnalysisDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case WM_TIMER_ONGETMEMORYINFO:
		OnGetProcessMemory();
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMemoryAnalysisDlg::OnBnClickedButtonMemoryFind()
{
	CString csProcessID = _T("");
	DWORD dwProcessID = 0;

	m_cProcessIDEdt.GetWindowTextW(csProcessID);

	if (csProcessID.IsEmpty())
	{
		MessageBoxW(_T("进程PID不能为空!"), _T(""), MB_OK | MB_ICONWARNING);
		return;
	}

	dwProcessID = _ttol(csProcessID);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwProcessID);

	if (NULL == hProcess)
	{
		MessageBoxW(_T("获取进程句柄失败!"), _T(""), MB_OK | MB_ICONWARNING);
		return;
	}

	CString csProcessHandle = _T("");

	csProcessHandle.Format(_T("%.8x"), hProcess);
	m_cProcessHandleEdt.SetWindowTextW(csProcessHandle);

	//char chFileName[MAX_PATH] = { 0 };
	//GetModuleFileNameExA(hProcess, NULL, chFileName, MAX_PATH);
	//Sleep(100);
}


void CMemoryAnalysisDlg::OnBnClickedButtonMemoryStart()
{
	if (!m_bStart)
	{
		::SetTimer(this->GetSafeHwnd(), WM_TIMER_ONGETMEMORYINFO, 100, NULL);
		m_cStartBtn.SetWindowTextW(_T("关闭"));
		m_bStart = true;
	}
	else
	{
		::KillTimer(this->GetSafeHwnd(), WM_TIMER_ONGETMEMORYINFO);
		m_cStartBtn.SetWindowTextW(_T("开启"));
		m_bStart = false;
	}
}


void CMemoryAnalysisDlg::OnBnClickedButtonMemoryExportdata()
{
	CurveExportData();
}


void CMemoryAnalysisDlg::OnBnClickedButtonMemoryExportpicture()
{
	CurveExportPicture();
}


void CMemoryAnalysisDlg::OnBnClickedButtonMemoryClear()
{
	m_vecMemory.clear();

	m_cChartCtrl.EnableRefresh(false);
	m_cChartCtrl.RemoveAllSeries();
	m_cChartCtrl.EnableRefresh(true);
}
