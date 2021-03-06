#pragma once

#include "CSerialPortBase.h"


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

// 成员
private:
	CRect m_cWindowRect;

private:
	CBrush m_Brush;

// 串口类变量
public:
	CCSerialPortBase m_cSerialPort;

// 成员函数
public:
	void ConstructionExtra();
	void InitWindowSharp();
	void InitControls();
	void RePaintWindow(CDC& dc);

private:
	bool SerialPortConfigCheck();
	void SerialPortSetValue(S_SERIALPORT_PROPERTY* pSerialPortInfo);
	void SerialPortSetOpenStatus();
	void SerialPortSetCloseStatus();
	bool SerialPortStartTransThread();
	bool SerialPortStartRecvThread();
	void SerialPortCloseTransThread();
	void SerialPortCloseRecvThread();
	void SerialPortGetRecvSpeed(DWORD& nSize);
	void SerialPortClearRecvSpeed();

protected:
	void SerialPortRecvOnTimer();
	void SerialPortSendOnTimer();

public:
	void BreakSpace(const unsigned char* pStr, vector<string>& vecStr);
	void Convert2Hex(vector<string>& vecStr, unsigned char* pStr, int nSize);

// 传输计数
public:
	DWORD m_dwSpeedCount;
	DWORD m_dwTranCount;
	DWORD m_dwRecvCount;

// 线程互斥
public:
	bool m_bShareInfo;
	CRITICAL_SECTION m_csThreadSafe;

// 串口数据
public:
	unsigned char m_chSendBuf[SERIALPORT_COMM_INPUT_BUFFER_SIZE];
	unsigned char m_chRecvBuf[SERIALPORT_COMM_OUTPUT_BUFFER_SIZE];
	DWORD m_dwSendSize;
	DWORD m_dwRecvSize;

// 线程变量
public:
	HANDLE m_hTran;
	HANDLE m_hRecv;

// 串口线程
public:
	static unsigned int CALLBACK OnTranslateBuffer(LPVOID lpParameters);
	static unsigned int CALLBACK OnReceiveBuffer(LPVOID lpParameters);

// 自定义函数
public:
	LRESULT OnRecvSerialPortBufferMsg(WPARAM wParam, LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckSerialSendTimer();
	CComboBox m_cSerialPortNameCbx;
	CComboBox m_cSerialPortBaudCbx;
	CComboBox m_cSerialPortDataBitCbx;
	CComboBox m_cSerialPortStopBitCbx;
	CComboBox m_cSerialPortCheckBitCbx;
	CButton m_cSerialPortRefreshBtn;
	CButton m_cSerialPortOpenBtn;
	CButton m_cSerialHelpRecvTextRdb;
	CButton m_cSerialHelpRecvHexRdb;
	CButton m_cSerialHelpSendTextRdb;
	CButton m_cSerialHelpSendHexRdb;
	CButton m_cSerialHelpClearRecvBtn;
	CButton m_cSerialHelpClearSendBtn;
	CEdit m_cSerialRecvEdt;
	CEdit m_cSerialSendEdt;
	CStatic m_cSerialRecvSpeedStic;
	CButton m_cSerialRecvLineCbx;
	CButton m_cSerialSendTimerCbx;
	CEdit m_cSerialSendTimerValueEdt;
	CButton m_cSerialSendBtn;
	CEdit m_cSerialInfoEdt;
	CEdit m_cSerialInfoPortEdt;
	CEdit m_cSerialInfoBaudEdt;
	CStatic m_cSerialInfoAllRecvStic;
	CStatic m_cSerialInfoAllSendStic;
	afx_msg void OnBnClickedButtonSerialRefresh();
	afx_msg void OnBnClickedButtonSerialOpen();
	afx_msg void OnBnClickedButtonSerialClearReceive();
	afx_msg void OnBnClickedButtonSerialClearSend();
	afx_msg void OnBnClickedButtonSerialSend();
};
