#pragma once


// CNetWorkDlg 对话框

class CNetWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNetWorkDlg)

public:
	CNetWorkDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNetWorkDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETWORK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
