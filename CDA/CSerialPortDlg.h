#pragma once


// CSerialPortDlg 对话框

class CSerialPortDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialPortDlg)

public:
	CSerialPortDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSerialPortDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
