// CAboutExDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CAboutExDlg.h"
#include "afxdialogex.h"


// CAboutExDlg 对话框

IMPLEMENT_DYNAMIC(CAboutExDlg, CDialogEx)

CAboutExDlg::CAboutExDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUT_DIALOG, pParent)
{

}

CAboutExDlg::~CAboutExDlg()
{
}

void CAboutExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutExDlg, CDialogEx)
END_MESSAGE_MAP()


// CAboutExDlg 消息处理程序
