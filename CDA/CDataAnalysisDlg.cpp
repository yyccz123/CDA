// CDataAnalysisDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CDataAnalysisDlg.h"
#include "afxdialogex.h"


// CDataAnalysisDlg 对话框

IMPLEMENT_DYNAMIC(CDataAnalysisDlg, CDialogEx)

CDataAnalysisDlg::CDataAnalysisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATAANALYSIS_DIALOG, pParent)
{

}

CDataAnalysisDlg::~CDataAnalysisDlg()
{
}

void CDataAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataAnalysisDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataAnalysisDlg 消息处理程序
