#pragma once


// CMemoryAnalysisDlg 对话框

class CMemoryAnalysisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMemoryAnalysisDlg)

public:
	CMemoryAnalysisDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMemoryAnalysisDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MEMERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
