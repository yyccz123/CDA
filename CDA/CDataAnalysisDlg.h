#pragma once


// CDataAnalysisDlg 对话框

class CDataAnalysisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataAnalysisDlg)

public:
	CDataAnalysisDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDataAnalysisDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATAANALYSIS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
