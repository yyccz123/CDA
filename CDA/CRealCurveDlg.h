#pragma once

#include "CSerialPortBase.h"

#include "ChartCtrl.h"
#include "ChartTitle.h"
#include "ChartAxisLabel.h"
#include "ChartBarSerie.h"
#include "ChartLineSerie.h"

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	CChartCtrl m_cChartCtrl;
	CComboBox m_cSerialPortNameCbx;
	CComboBox m_cSerialPortBaudCbx;
	CComboBox m_cSerialPortDataBitCbx;
	CComboBox m_cSerialPortStopBitCbx;
	CComboBox m_cSerialPortCheckBitCbx;
	CButton m_cSerialPortRefreshBtn;
	CButton m_cSerialPortOpenBtn;
	CButton m_cCurve1Cbx;
	CButton m_cCurve2Cbx;
	CButton m_cCurve3Cbx;
	CButton m_cCurve4Cbx;
	CButton m_cCurve5Cbx;
	CButton m_cCurve6Cbx;
	CButton m_cCurve7Cbx;
	CButton m_cCurve8Cbx;
	CStatic m_cCurve1Stic;
	CStatic m_cCurve2Stic;
	CStatic m_cCurve3Stic;
	CStatic m_cCurve4Stic;
	CStatic m_cCurve5Stic;
	CStatic m_cCurve6Stic;
	CStatic m_cCurve7Stic;
	CStatic m_cCurve8Stic;
	CButton m_cCurveClearBtn;
	CEdit m_cSerialInfoEdt;
	CEdit m_cSerialInfoPortEdt;
	CEdit m_cSerialInfoBaudEdt;
	CStatic m_cSerialInfoAllRecvStic;
	CButton m_cCurveExportBtn;
	afx_msg void OnBnClickedButtonCurveRefresh();
	afx_msg void OnBnClickedButtonCurveOpen();
	afx_msg void OnBnClickedButtonCurveClear();
	afx_msg void OnBnClickedButtonCurveExportData();
};
