#pragma once

#include "CSocketBase.h"
#include "CThreadSafe.h"
#include "CThreadSafeEx.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

// CTCPServerDlg 对话框
#define SAFE_RELEASE(Pointer)	{if(Pointer){(Pointer)->Release();(Pointer) = NULL;}}
#define SAFE_DELETE(Pointer)	{if(Pointer){delete(Pointer);(Pointer) = NULL;}}
#define SAFE_DELETE_ARRAY(Pointer) {if(Pointer){delete[](Pointer);(Pointer) = NULL;}}

class CTCPServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTCPServerDlg)

public:
	CTCPServerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTCPServerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TCPSERVER };
#endif

// 成员
private:
	CRect m_cWindowRect;

private:
	CBrush m_Brush;

// 网络通信类
private:
	CCSocketBase* m_pSocket;
	CThreadSafeEx m_csThreadSafe;

// 服务端相关
public:
	S_CLIENTINFO m_sClientInfo;

// 客户端相关
public:
	int m_nClientNo;
	map<int, S_CLIENTINFO> m_mapClientInfo;

// 数据收发
public:
	unsigned char m_chSendBuf[SOB_TCP_SEND_BUFFER];
	unsigned char m_chRecvBuf[SOB_TCP_RECV_BUFFER];
	UINT m_uSendCount;
	UINT m_uRecvCount;

// 成员函数
public:
	void ConstructionExtra();
	void InitWindowSharp();
	void InitControls();
	void RePaintWindow(CDC& dc);

// 网络通信相关
public:
	void InitSocket();
	void ReleaseSocket();

// 网络通信线程变量
public:
	HANDLE m_hAccept;
	volatile bool m_bListen;
	BOOL m_bListenExit;

	bool m_bExit;
	bool m_bShareInfo;

// 网络通信接收线程
public:
	void StartAcceptThread();
	static unsigned int CALLBACK OnWaitAcceptThread(LPVOID lpParameters);
	static unsigned int CALLBACK OnHandleAcceptThread(LPVOID lpParameters);

private:
	void BreakIpAddress(const char* pArr, BYTE byArr[4]);
	void BreakSpace(const unsigned char* pStr, vector<string>& vecStr);
	void Convert2Hex(vector<string>& vecStr, unsigned char* pStr, int nSize);

// 自定义函数
public:
	LRESULT OnRecvSocketBufferMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnClientConnectServerMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnClientDisConnectServerMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateSendObject(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateClient(WPARAM wParam, LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	CIPAddressCtrl m_cHostIPAdc;
	CEdit m_cHostPortEdt;
	CEdit m_cServerLinkMaxEdt;
	CButton m_cListenBtn;
	CButton m_cRecvHexCbx;
	CButton m_cSendHexCbx;
	CButton m_cRecvClearBtn;
	CButton m_cSendClearBtn;
	CEdit m_cRecvEdt;
	CEdit m_cSendEdt;
	CComboBox m_cObjectCbx;
	CButton m_cSendBtn;
	afx_msg void OnBnClickedButtonNetServerListen();
	afx_msg void OnBnClickedButtonNetServerRecvClear();
	afx_msg void OnBnClickedButtonNetServerSendClear();
	afx_msg void OnBnClickedButtonNetServerSend();
};

// Variable Definition
extern CTCPServerDlg* g_pTCPServerDlg;