// CMemoryAnalysisDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CMemoryAnalysisDlg.h"
#include "afxdialogex.h"


// CMemoryAnalysisDlg 对话框

IMPLEMENT_DYNAMIC(CMemoryAnalysisDlg, CDialogEx)

CMemoryAnalysisDlg::CMemoryAnalysisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MEMERY, pParent)
{

}

CMemoryAnalysisDlg::~CMemoryAnalysisDlg()
{
}

void CMemoryAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMemoryAnalysisDlg, CDialogEx)
END_MESSAGE_MAP()


// CMemoryAnalysisDlg 消息处理程序
