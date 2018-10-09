#pragma once

#include "ChartCtrl.h"
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

// 成员函数
public:
	void ConstructionExtra();
	void InitWindowSharp();
	void InitControls();
	void RePaintWindow(CDC& dc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	CChartCtrl m_cChartCtrl;
};
