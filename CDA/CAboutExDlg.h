#pragma once


// CAboutExDlg 对话框

class CAboutExDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutExDlg)

public:
	CAboutExDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAboutExDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
