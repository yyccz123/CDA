// CSerialPortDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CSerialPortDlg.h"
#include "afxdialogex.h"


// CSerialPortDlg 对话框

IMPLEMENT_DYNAMIC(CSerialPortDlg, CDialogEx)

CSerialPortDlg::CSerialPortDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALPORT_DIALOG, pParent)
{

}

CSerialPortDlg::~CSerialPortDlg()
{
}

void CSerialPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSerialPortDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialPortDlg 消息处理程序
