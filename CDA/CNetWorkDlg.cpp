// CNetWorkDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CNetWorkDlg.h"
#include "afxdialogex.h"


// CNetWorkDlg 对话框

IMPLEMENT_DYNAMIC(CNetWorkDlg, CDialogEx)

CNetWorkDlg::CNetWorkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NETWORK_DIALOG, pParent)
{

}

CNetWorkDlg::~CNetWorkDlg()
{
}

void CNetWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetWorkDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetWorkDlg 消息处理程序
