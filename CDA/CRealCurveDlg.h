#pragma once


// CRealCurveDlg 对话框

class CRealCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRealCurveDlg)

public:
	CRealCurveDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRealCurveDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REALCURVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
