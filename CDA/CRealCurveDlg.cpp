// CRealCurveDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CRealCurveDlg.h"
#include "afxdialogex.h"


// CRealCurveDlg 对话框

IMPLEMENT_DYNAMIC(CRealCurveDlg, CDialogEx)

CRealCurveDlg::CRealCurveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REALCURVE_DIALOG, pParent)
{

}

CRealCurveDlg::~CRealCurveDlg()
{
}

void CRealCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRealCurveDlg, CDialogEx)
END_MESSAGE_MAP()


// CRealCurveDlg 消息处理程序
