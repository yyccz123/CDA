// CSerialPortDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CSerialPortDlg.h"
#include "afxdialogex.h"
#include "CThreadSafe.h"

//Macro Definition
#define WM_USER_TIMER_ONREFRESHSPEED	0
#define WM_USER_TIMER_ONTIMERSENDDATA	1

#define WM_USER_MSG_ONRECEIVEBUFFER		(WM_USER + 1)

// CSerialPortDlg 对话框

IMPLEMENT_DYNAMIC(CSerialPortDlg, CDialogEx)

CSerialPortDlg::CSerialPortDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALPORT_DIALOG, pParent)
{
	ConstructionExtra();
}

CSerialPortDlg::~CSerialPortDlg()
{
	m_Brush.DeleteObject();
}

// CSerialPortDlg 构造函数
void CSerialPortDlg::ConstructionExtra()
{
	m_hTran = NULL;
	m_hRecv = NULL;

	m_dwSpeedCount = 0;
	m_dwTranCount = 0;
	m_dwRecvCount = 0;
	m_dwSendSize = 0;
	m_dwRecvSize = 0;
	memset(m_chSendBuf, 0, sizeof(m_chSendBuf));
	memset(m_chRecvBuf, 0, sizeof(m_chRecvBuf));

	m_bShareInfo = false;
	InitializeCriticalSection(&m_csThreadSafe);

	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

// CSerialPortDlg 初始化窗口形状
void CSerialPortDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

// CSerialPortDlg 初始化控件
void CSerialPortDlg::InitControls()
{
	m_cSerialPort.EnumSerialPort();		// 枚举串口

	//<<< 串口配置

	// 添加串口
	m_cSerialPortNameCbx.ResetContent();	// 清空Cbx
	for (auto iter = m_cSerialPort.m_mapEnumCOM.begin(); iter != m_cSerialPort.m_mapEnumCOM.end(); ++iter)
	{
		USES_CONVERSION;
		m_cSerialPortNameCbx.InsertString(iter->first, A2T((iter->second).c_str()));
	}

	m_cSerialPortNameCbx.SetCurSel(0);

	// 添加波特率
	m_cSerialPortBaudCbx.ResetContent();	// 清空Cbx
	m_cSerialPortBaudCbx.InsertString(0, _T("9600"));
	m_cSerialPortBaudCbx.InsertString(1, _T("14400"));
	m_cSerialPortBaudCbx.InsertString(2, _T("19200"));
	m_cSerialPortBaudCbx.InsertString(3, _T("38400"));
	m_cSerialPortBaudCbx.InsertString(4, _T("57600"));
	m_cSerialPortBaudCbx.InsertString(5, _T("115200"));
	m_cSerialPortBaudCbx.InsertString(6, _T("128000"));
	m_cSerialPortBaudCbx.InsertString(7, _T("256000"));
	m_cSerialPortBaudCbx.SetCurSel(5);

	// 添加数据位
	m_cSerialPortDataBitCbx.ResetContent();	// 清空Cbx
	m_cSerialPortDataBitCbx.InsertString(0, _T("5"));
	m_cSerialPortDataBitCbx.InsertString(1, _T("6"));
	m_cSerialPortDataBitCbx.InsertString(2, _T("7"));
	m_cSerialPortDataBitCbx.InsertString(3, _T("8"));
	m_cSerialPortDataBitCbx.SetCurSel(3);

	// 添加停止位
	m_cSerialPortStopBitCbx.ResetContent();	// 清空Cbx
	m_cSerialPortStopBitCbx.InsertString(0, _T("1"));
	m_cSerialPortStopBitCbx.InsertString(1, _T("2"));
	m_cSerialPortStopBitCbx.SetCurSel(0);

	// 添加校验位
	m_cSerialPortCheckBitCbx.ResetContent();	// 清空Cbx
	m_cSerialPortCheckBitCbx.InsertString(0, _T("无校验"));
	m_cSerialPortCheckBitCbx.InsertString(1, _T("奇校验"));
	m_cSerialPortCheckBitCbx.InsertString(2, _T("偶校验"));
	m_cSerialPortCheckBitCbx.SetCurSel(0);

	//<<< 串口辅助

	// 收码区
	m_cSerialHelpRecvTextRdb.SetCheck(TRUE);
	m_cSerialHelpRecvHexRdb.SetCheck(FALSE);

	// 发码区
	m_cSerialHelpSendTextRdb.SetCheck(TRUE);
	m_cSerialHelpSendHexRdb.SetCheck(FALSE);
	
	//<<< 接收区


	//<<< 发送区
	m_cSerialSendTimerValueEdt.SetWindowTextW(_T("1000"));	// 默认定时发送时长1000ms

	//<<< 串口提示信息
	if (!m_cSerialPort.m_mapEnumCOM.empty())
	{
		CString csPort;
		CString csBaud;

		m_cSerialPortNameCbx.GetLBText(m_cSerialPortNameCbx.GetCurSel(), csPort);
		m_cSerialPortBaudCbx.GetLBText(m_cSerialPortBaudCbx.GetCurSel(), csBaud);
		m_cSerialInfoEdt.SetWindowTextW(_T("已检测到串口!"));
		m_cSerialInfoPortEdt.SetWindowTextW(csPort);
		m_cSerialInfoBaudEdt.SetWindowTextW(csBaud);
	}
	else
	{
		m_cSerialInfoEdt.SetWindowTextW(_T("未检测到串口!"));
		m_cSerialInfoPortEdt.SetWindowTextW(_T(""));
		m_cSerialInfoBaudEdt.SetWindowTextW(_T(""));
	}

}

// CSerialPortDlg 重绘窗口
void CSerialPortDlg::RePaintWindow(CDC & dc)
{
	CDC MemDC;
	CBitmap MemBitmap;
	CRect MemRect;

	GetClientRect(&MemRect);

	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, MemRect.Width(), MemRect.Height());

	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(&MemRect, RGB(255, 255, 255));

	dc.BitBlt(m_cWindowRect.left, m_cWindowRect.top, m_cWindowRect.Width(), m_cWindowRect.Height(), &MemDC, MemRect.left, MemRect.top, SRCCOPY);

	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

// CSerialPortDlg 检查串口设置
bool CSerialPortDlg::SerialPortConfigCheck()
{
	CString csSerialPortName;
	CString csSerialPortBaud;
	CString csSerialPortDataBits;
	CString csSerialPortStopBits;
	CString csSerialPortCheckBits;

	m_cSerialPortNameCbx.GetWindowTextW(csSerialPortName);
	m_cSerialPortBaudCbx.GetWindowTextW(csSerialPortBaud);
	m_cSerialPortDataBitCbx.GetWindowTextW(csSerialPortDataBits);
	m_cSerialPortStopBitCbx.GetWindowTextW(csSerialPortStopBits);
	m_cSerialPortCheckBitCbx.GetWindowTextW(csSerialPortCheckBits);

	if (_T("") == csSerialPortName)
	{
		MessageBoxW(_T("串口名未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}
	
	if (_T("") == csSerialPortBaud)
	{
		MessageBoxW(_T("串口波特率未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortDataBits)
	{
		MessageBoxW(_T("串口数据位未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortStopBits)
	{
		MessageBoxW(_T("串口停止位未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	if (_T("") == csSerialPortCheckBits)
	{
		MessageBoxW(_T("串口校验位未设置!"), _T("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

// CSerialPortDlg 填充初始化结构体
void CSerialPortDlg::SerialPortSetValue(S_SERIALPORT_PROPERTY * pSerialPortInfo)
{
	CString csSerialPortName;
	CString csSerialPortBaud;
	CString csSerialPortDataBits;
	CString csSerialPortStopBits;
	CString csSerialPortCheckBits;

	m_cSerialPortNameCbx.GetWindowTextW(csSerialPortName);
	m_cSerialPortBaudCbx.GetWindowTextW(csSerialPortBaud);
	m_cSerialPortDataBitCbx.GetWindowTextW(csSerialPortDataBits);
	m_cSerialPortStopBitCbx.GetWindowTextW(csSerialPortStopBits);
	m_cSerialPortCheckBitCbx.GetWindowTextW(csSerialPortCheckBits);

	USES_CONVERSION;
	strcpy_s(pSerialPortInfo->chPort, T2A(csSerialPortName));
	pSerialPortInfo->dwBaudRate = _ttol(csSerialPortBaud);
	pSerialPortInfo->byDataBits = _ttoi(csSerialPortDataBits);

	if (1 == _ttoi(csSerialPortStopBits))
	{
		pSerialPortInfo->byStopBits = 0;
	}
	else if (2 == _ttoi(csSerialPortStopBits))
	{
		pSerialPortInfo->byStopBits = 2;
	}

	if (!strcmp("无校验", T2A(csSerialPortCheckBits)))
	{
		pSerialPortInfo->byCheckBits = 0;
	}
	else if (!strcmp("奇校验", T2A(csSerialPortCheckBits)))
	{
		pSerialPortInfo->byCheckBits = 1;
	}
	else if (!strcmp("偶校验", T2A(csSerialPortCheckBits)))
	{
		pSerialPortInfo->byCheckBits = 2;
	}

}

// CSerialPortDlg 设置打开串口窗口显示
void CSerialPortDlg::SerialPortSetOpenStatus()
{
	CString csPort;
	CString csBaud;

	// 打开串口按钮显示
	m_cSerialPortOpenBtn.SetWindowTextW(_T("关闭串口"));

	// 禁止按钮选择
	m_cSerialPortRefreshBtn.EnableWindow(FALSE);

	m_cSerialPortNameCbx.EnableWindow(FALSE);
	m_cSerialPortBaudCbx.EnableWindow(FALSE);
	m_cSerialPortDataBitCbx.EnableWindow(FALSE);
	m_cSerialPortStopBitCbx.EnableWindow(FALSE);
	m_cSerialPortCheckBitCbx.EnableWindow(FALSE);

	m_cSerialHelpRecvTextRdb.EnableWindow(FALSE);
	m_cSerialHelpRecvHexRdb.EnableWindow(FALSE);
	m_cSerialHelpSendTextRdb.EnableWindow(FALSE);
	m_cSerialHelpSendHexRdb.EnableWindow(FALSE);

	// 串口信息提示
	m_cSerialPortNameCbx.GetLBText(m_cSerialPortNameCbx.GetCurSel(), csPort);
	m_cSerialPortBaudCbx.GetLBText(m_cSerialPortBaudCbx.GetCurSel(), csBaud);
	m_cSerialInfoEdt.SetWindowTextW(_T("串口已打开!"));
	m_cSerialInfoPortEdt.SetWindowTextW(csPort);
	m_cSerialInfoBaudEdt.SetWindowTextW(csBaud);

}

// CSerialPortDlg 设置关闭串口窗口显示
void CSerialPortDlg::SerialPortSetCloseStatus()
{
	CString csPort;
	CString csBaud;

	// 打开串口按钮显示
	m_cSerialPortOpenBtn.SetWindowTextW(_T("打开串口"));

	// 禁止按钮选择
	m_cSerialPortRefreshBtn.EnableWindow(TRUE);

	m_cSerialPortNameCbx.EnableWindow(TRUE);
	m_cSerialPortBaudCbx.EnableWindow(TRUE);
	m_cSerialPortDataBitCbx.EnableWindow(TRUE);
	m_cSerialPortStopBitCbx.EnableWindow(TRUE);
	m_cSerialPortCheckBitCbx.EnableWindow(TRUE);

	m_cSerialHelpRecvTextRdb.EnableWindow(TRUE);
	m_cSerialHelpRecvHexRdb.EnableWindow(TRUE);
	m_cSerialHelpSendTextRdb.EnableWindow(TRUE);
	m_cSerialHelpSendHexRdb.EnableWindow(TRUE);

	// 串口信息提示
	m_cSerialPortNameCbx.GetLBText(m_cSerialPortNameCbx.GetCurSel(), csPort);
	m_cSerialPortBaudCbx.GetLBText(m_cSerialPortBaudCbx.GetCurSel(), csBaud);
	m_cSerialInfoEdt.SetWindowTextW(_T("串口已关闭!"));
	m_cSerialInfoPortEdt.SetWindowTextW(csPort);
	m_cSerialInfoBaudEdt.SetWindowTextW(csBaud);

}

// CSerialPortDlg 开始串口发送线程
bool CSerialPortDlg::SerialPortStartTransThread()
{
	unsigned int uThreadID;

	m_hTran = (HANDLE)::_beginthreadex(NULL, 0, (_beginthreadex_proc_type)OnTranslateBuffer, this, 0, &uThreadID);
	if (!m_hTran)
	{
		return false;
	}

	return true;
}

// CSerialPortDlg 开始串口接收线程
bool CSerialPortDlg::SerialPortStartRecvThread()
{
	unsigned int uThreadID;

	m_hRecv = (HANDLE)::_beginthreadex(NULL, 0, (_beginthreadex_proc_type)OnReceiveBuffer, this, 0, &uThreadID);
	if (!m_hRecv)
	{
		return false;
	}

	return true;
}

// CSerialPortDlg 关闭串口发送线程
void CSerialPortDlg::SerialPortCloseTransThread()
{
	if (NULL != m_hTran)
	{
		::WaitForSingleObject(m_hTran, INFINITE);
		::CloseHandle(m_hTran);
		m_hTran = NULL;
	}

}

// CSerialPortDlg 关闭串口接收线程
void CSerialPortDlg::SerialPortCloseRecvThread()
{
	if (NULL != m_hRecv)
	{
		m_bShareInfo = false;
		::WaitForSingleObject(m_hRecv, INFINITE);
		::CloseHandle(m_hRecv);
		m_hRecv = NULL;
	}

}

// CSerialPortDlg 串口接收速度计算
void CSerialPortDlg::SerialPortGetRecvSpeed(DWORD& nSize)
{
	static DWORD dwLastTime = 0;
	DWORD dwNowTime = 0;
	float fElapsedTime = 0.0f;

	timeBeginPeriod(1);
	dwNowTime = timeGetTime();
	timeEndPeriod(1);
	fElapsedTime = (dwNowTime - dwLastTime) * 0.001f;

	if (fElapsedTime >= 1.0f)
	{
		CString csRecvSpeed;
		float fSpeed = 0.0f;

		fSpeed = (float)(nSize * 1.0f / fElapsedTime);
		dwLastTime = dwNowTime;
		nSize = 0;

		csRecvSpeed.Format(_T("速度:%d字节/秒"), (int)fSpeed);
		m_cSerialRecvSpeedStic.SetWindowTextW(csRecvSpeed);
		m_cSerialRecvSpeedStic.Invalidate(FALSE);
		m_cSerialRecvSpeedStic.UpdateWindow();
	}

}

// CSerialPortDlg 清除串口传输速度
void CSerialPortDlg::SerialPortClearRecvSpeed()
{
	m_cSerialRecvSpeedStic.SetWindowTextW(_T("速度:0字节/秒"));
	m_cSerialRecvSpeedStic.Invalidate(FALSE);
	m_cSerialRecvSpeedStic.UpdateWindow();
}

// CSerialPortDlg 串口接收定时器计算
void CSerialPortDlg::SerialPortRecvOnTimer()
{
	// 显示串口接收数据
	CString csRecvData;

	csRecvData.Format(_T("已接收:%ld"), m_dwRecvCount);
	m_cSerialInfoAllRecvStic.SetWindowTextW(csRecvData);
	m_cSerialInfoAllRecvStic.Invalidate(FALSE);
	m_cSerialInfoAllRecvStic.UpdateWindow();

	// 显示串口发送数据
	CString csTranData;

	csTranData.Format(_T("已发送:%ld"), m_dwTranCount);
	m_cSerialInfoAllSendStic.SetWindowTextW(csTranData);
	m_cSerialInfoAllSendStic.Invalidate(FALSE);
	m_cSerialInfoAllSendStic.UpdateWindow();

	// 显示串口传输速度
	SerialPortGetRecvSpeed(m_dwSpeedCount);

}

// CSerialPortDlg 串口发送定时器
void CSerialPortDlg::SerialPortSendOnTimer()
{
	if (NULL != m_hTran)
	{
		::WaitForSingleObject(m_hTran, INFINITE);
		::CloseHandle(m_hTran);
		m_hTran = NULL;
	}

	SerialPortStartTransThread();
}

// CSerialPortDlg 拆分空格
void CSerialPortDlg::BreakSpace(const unsigned char * pStr, vector<string>& vecStr)
{
	unsigned char* pString = const_cast<unsigned char*>(pStr);
	char* pNewString = nullptr;
	int nSize = 0;
	char* pTemp = nullptr;
	char* pArr = nullptr;

	nSize = strlen((char*)pString) + 1;
	pNewString = new char[nSize];
	memset(pNewString, 0, nSize);
	memcpy_s(pNewString, nSize, pString, strlen((char*)pString));

	vecStr.clear();

	pTemp = strtok_s(pNewString, " ", &pArr);
	while (pTemp)
	{
		vecStr.push_back(pTemp);
		pTemp = strtok_s(NULL, " ", &pArr);
	}

	delete[] pNewString;
	pNewString = nullptr;
}

// CSerialPortDlg 16进制转换
void CSerialPortDlg::Convert2Hex(vector<string>& vecStr, unsigned char * pStr, int nSize)
{
	char* pArr = nullptr;
	int nDelta = 0;

	pArr = new char[nSize];
	memset(pArr, 0, nSize);

	for (auto iter = vecStr.begin(); iter != vecStr.end(); ++iter)
	{
		char* pTemp = const_cast<char*>((*iter).c_str());
		for (size_t i = 0; (i <= (*iter).size()) && ((i + 1) <= (*iter).size()); i += 2, pTemp += 2)
		{
			if (((*pTemp >= '0' && *pTemp <= '9') || (*pTemp >= 'A' && *pTemp <= 'F') || (*pTemp >= 'a' && *pTemp <= 'f'))
				&& ((*(pTemp + 1) >= '0' && *(pTemp + 1) <= '9') || (*(pTemp + 1) >= 'A' && *(pTemp + 1) <= 'F') || (*(pTemp + 1) >= 'a' && *(pTemp + 1) <= 'f')))
			{
				char ch = 0;
				char cl = 0;

				if (*pTemp >= '0' && *pTemp <= '9')
				{
					ch = *pTemp - 48;
				}
				else if (*pTemp >= 'A' && *pTemp <= 'F')
				{
					ch = *pTemp - 55;
				}
				else
				{
					ch = *pTemp - 87;
				}

				if (*(pTemp + 1) >= '0' && *(pTemp + 1) <= '9')
				{
					cl = *(pTemp + 1) - 48;
				}
				else if (*(pTemp + 1) >= 'A' && *(pTemp + 1) <= 'F')
				{
					cl = *(pTemp + 1) - 55;
				}
				else
				{
					cl = *(pTemp + 1) - 87;
				}

				*(pArr + nDelta) = ch * 16 + cl;
				nDelta++;
				if (nDelta >= nSize)
				{
					break;
				}

			}

		}

	}

	m_dwSendSize = nDelta;
	memcpy_s(pStr, nSize, pArr, nSize);

	delete[] pArr;
	pArr = nullptr;
}

// CSerialPortDlg 串口发送线程
unsigned int CALLBACK CSerialPortDlg::OnTranslateBuffer(LPVOID lpParameters)
{
	CSerialPortDlg* pUser = reinterpret_cast<CSerialPortDlg*>(lpParameters);
	CString csTrans;

	USES_CONVERSION;

	pUser->m_cSerialSendEdt.GetWindowTextW(csTrans);
	memset(pUser->m_chSendBuf, 0, sizeof(pUser->m_chSendBuf));
	strcpy_s((char*)pUser->m_chSendBuf, sizeof(pUser->m_chSendBuf), T2A(csTrans));

	// 16进制发送数据
	if (TRUE == pUser->m_cSerialHelpSendHexRdb.GetCheck())
	{
		vector<string> vecSendBuf;

		pUser->BreakSpace(pUser->m_chSendBuf, vecSendBuf);
		pUser->m_dwSendSize = vecSendBuf.size();
		memset(pUser->m_chSendBuf, 0, sizeof(pUser->m_chSendBuf));
		pUser->Convert2Hex(vecSendBuf, pUser->m_chSendBuf, sizeof(pUser->m_chSendBuf));
	}
	else
	{
		pUser->m_dwSendSize = strlen((char*)pUser->m_chSendBuf);
	}

	// 发送串口数据
	pUser->m_cSerialPort.CCSerialPortBaseSetSendBuf(pUser->m_chSendBuf, sizeof(pUser->m_chSendBuf), pUser->m_dwSendSize);
	pUser->m_cSerialPort.OnTranslateBuffer();

	// 发送数据统计
	pUser->m_dwTranCount += pUser->m_dwSendSize;
	if (pUser->m_dwTranCount > 65535)
	{
		pUser->m_dwTranCount = 0;
	}

	return 0;
}

// CSerialPortDlg 串口接收线程
unsigned int CALLBACK CSerialPortDlg::OnReceiveBuffer(LPVOID lpParameters)
{
	CSerialPortDlg* pUser = reinterpret_cast<CSerialPortDlg*>(lpParameters);

	while (true)
	{
		EnterCriticalSection(&pUser->m_cSerialPort.m_csCOMSync);
		if (!pUser->m_cSerialPort.m_bOpen)
		{
			LeaveCriticalSection(&pUser->m_cSerialPort.m_csCOMSync);
			break;
		}
		LeaveCriticalSection(&pUser->m_cSerialPort.m_csCOMSync);

		while (true)
		{
			EnterCriticalSection(&pUser->m_csThreadSafe);
			if (!pUser->m_bShareInfo)
			{
				pUser->m_bShareInfo = true;
				LeaveCriticalSection(&pUser->m_csThreadSafe);
				break;
			}
			LeaveCriticalSection(&pUser->m_csThreadSafe);
			Sleep(10);
		}

		if (pUser->m_cSerialPort.CCSerialPortBaseGetRecv())
		{
			memset(pUser->m_chRecvBuf, 0, sizeof(pUser->m_chRecvBuf));
			pUser->m_cSerialPort.CCSerialPortBaseGetRecvBuf(pUser->m_chRecvBuf, sizeof(pUser->m_chRecvBuf), pUser->m_dwRecvSize);
			pUser->m_cSerialPort.CCSerialPortBaseSetRecv(false);
			::PostMessageA(pUser->m_hWnd, WM_USER_MSG_ONRECEIVEBUFFER, (WPARAM)((LPVOID)(&pUser->m_chRecvBuf)), (LPARAM)0);
		}
		else
		{
			pUser->m_bShareInfo = false;
		}
		
	}

	return 0;
}

// CSerialPortDlg 接收到串口数据
LRESULT CSerialPortDlg::OnRecvSerialPortBufferMsg(WPARAM wParam, LPARAM lParam)
{
	CThreadSafe ThreadSafe(&m_csThreadSafe);
	int nLen = 0;
	float fSpeed = 0.0f;

	USES_CONVERSION;

	// 选择16进制显示
	if (TRUE == m_cSerialHelpRecvHexRdb.GetCheck())
	{
		CString csRecvBuff = _T("");
		for (int i = 0; i < m_dwRecvSize; ++i)
		{
			CString csTemp = _T("");
			csTemp.Format(_T("%02X "), m_chRecvBuf[i]);
			csRecvBuff += csTemp;
		}

		memset(m_chRecvBuf, 0, sizeof(m_chRecvBuf));
		strcpy_s((char*)m_chRecvBuf, sizeof(m_chRecvBuf), T2A(csRecvBuff));
	}

	// 选择接收换行
	if (TRUE == m_cSerialRecvLineCbx.GetCheck())
	{
		strcat_s((char*)m_chRecvBuf, sizeof(m_chRecvBuf), "\r\n");
	}

	// 接收区文本显示
	nLen = m_cSerialRecvEdt.GetWindowTextLengthW();
	if (nLen >= 30000)
	{
		m_cSerialRecvEdt.SetWindowTextW(_T(""));
		nLen = -1;
	}

	m_cSerialRecvEdt.SetSel(nLen, nLen);
	m_cSerialRecvEdt.ReplaceSel(A2T((char*)m_chRecvBuf));

	// 接收数据统计
	if (TRUE == m_cSerialHelpRecvHexRdb.GetCheck())
	{
		m_dwRecvCount += m_dwRecvSize;
	}
	else
	{
		m_dwRecvCount += strlen((char*)m_chRecvBuf);
	}

	if (m_dwRecvCount > 65535)
	{
		m_dwRecvCount = 0;
	}

	// 速度数据统计
	if (TRUE == m_cSerialHelpRecvHexRdb.GetCheck())
	{
		m_dwSpeedCount += m_dwRecvSize;
	}
	else
	{
		m_dwSpeedCount += strlen((char*)m_chRecvBuf);
	}

	m_bShareInfo = false;
	return 0;
}

void CSerialPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERIAL_PORT, m_cSerialPortNameCbx);
	DDX_Control(pDX, IDC_COMBO_SERIAL_BOAD, m_cSerialPortBaudCbx);
	DDX_Control(pDX, IDC_COMBO_SERIAL_DATABIT, m_cSerialPortDataBitCbx);
	DDX_Control(pDX, IDC_COMBO_SERIAL_STOPBIT, m_cSerialPortStopBitCbx);
	DDX_Control(pDX, IDC_COMBO_SERIAL_CHECKBIT, m_cSerialPortCheckBitCbx);
	DDX_Control(pDX, IDC_BUTTON_SERIAL_REFRESH, m_cSerialPortRefreshBtn);
	DDX_Control(pDX, IDC_BUTTON_SERIAL_OPEN, m_cSerialPortOpenBtn);
	DDX_Control(pDX, IDC_RADIO_SERIAL_RECEIVE_TEXT, m_cSerialHelpRecvTextRdb);
	DDX_Control(pDX, IDC_RADIO_SERIAL_RECEIVE_HEX, m_cSerialHelpRecvHexRdb);
	DDX_Control(pDX, IDC_RADIO_SERIAL_SEND_TEXT, m_cSerialHelpSendTextRdb);
	DDX_Control(pDX, IDC_RADIO_SERIAL_SEND_HEX, m_cSerialHelpSendHexRdb);
	DDX_Control(pDX, IDC_BUTTON_SERIAL_CLEAR_RECEIVE, m_cSerialHelpClearRecvBtn);
	DDX_Control(pDX, IDC_BUTTON_SERIAL_CLEAR_SEND, m_cSerialHelpClearSendBtn);
	DDX_Control(pDX, IDC_EDIT_SERIAL_RECEIVE, m_cSerialRecvEdt);
	DDX_Control(pDX, IDC_EDIT_SERIAL_SEND, m_cSerialSendEdt);
	DDX_Control(pDX, IDC_STATIC_SERIAL_RECEIVE_SPEED, m_cSerialRecvSpeedStic);
	DDX_Control(pDX, IDC_CHECK_SERIAL_RECEIVE_CHANGELINE, m_cSerialRecvLineCbx);
	DDX_Control(pDX, IDC_CHECK_SERIAL_SEND_TIMER, m_cSerialSendTimerCbx);
	DDX_Control(pDX, IDC_EDIT_SERIAL_TIMER_VALUE, m_cSerialSendTimerValueEdt);
	DDX_Control(pDX, IDC_BUTTON_SERIAL_SEND, m_cSerialSendBtn);
	DDX_Control(pDX, IDC_EDIT_SERIAL_INFO_MESSAGE, m_cSerialInfoEdt);
	DDX_Control(pDX, IDC_EDIT_SERIAL_INFO_PORT, m_cSerialInfoPortEdt);
	DDX_Control(pDX, IDC_EDIT_SERIAL_INFO_BAUD, m_cSerialInfoBaudEdt);
	DDX_Control(pDX, IDC_STATIC_SERIAL_RECEIVE_ALL, m_cSerialInfoAllRecvStic);
	DDX_Control(pDX, IDC_STATIC_SERIAL_SEND_ALL, m_cSerialInfoAllSendStic);
}


BEGIN_MESSAGE_MAP(CSerialPortDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_REFRESH, &CSerialPortDlg::OnBnClickedButtonSerialRefresh)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_OPEN, &CSerialPortDlg::OnBnClickedButtonSerialOpen)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_CLEAR_RECEIVE, &CSerialPortDlg::OnBnClickedButtonSerialClearReceive)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_CLEAR_SEND, &CSerialPortDlg::OnBnClickedButtonSerialClearSend)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_MSG_ONRECEIVEBUFFER, &CSerialPortDlg::OnRecvSerialPortBufferMsg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_SEND, &CSerialPortDlg::OnBnClickedButtonSerialSend)
	ON_BN_CLICKED(IDC_CHECK_SERIAL_SEND_TIMER, &CSerialPortDlg::OnBnClickedCheckSerialSendTimer)
END_MESSAGE_MAP()


// CSerialPortDlg 消息处理程序

// CSerialPortDlg 窗口初始化
BOOL CSerialPortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();
	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// CSerialPortDlg 窗口重绘
void CSerialPortDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}

// CSerialPortDlg 设置控件颜色
HBRUSH CSerialPortDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		return m_Brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

// CSerialPortDlg 关闭窗口
void CSerialPortDlg::OnClose()
{
	::KillTimer(m_hWnd, WM_USER_TIMER_ONREFRESHSPEED);
	::KillTimer(m_hWnd, WM_USER_TIMER_ONTIMERSENDDATA);
	DeleteCriticalSection(&m_csThreadSafe);

	CDialogEx::OnClose();
}

// CSerialPortDlg 定时器响应
void CSerialPortDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case WM_USER_TIMER_ONREFRESHSPEED:
		SerialPortRecvOnTimer();
		break;
	case WM_USER_TIMER_ONTIMERSENDDATA:
		SerialPortSendOnTimer();
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// CSerialPortDlg 定时发送选中相应
void CSerialPortDlg::OnBnClickedCheckSerialSendTimer()
{
	if (TRUE == m_cSerialSendTimerCbx.GetCheck())
	{
		CString csTime = _T("");
		int nTime = 0;

		USES_CONVERSION;
		m_cSerialSendTimerValueEdt.GetWindowTextW(csTime);
		if (_T("") == csTime)
		{
			MessageBoxW(_T("定时时间不能为空!"), _T("警告"), MB_OK | MB_ICONWARNING);
			m_cSerialSendTimerCbx.SetCheck(FALSE);
			return;
		}

		nTime = atoi(T2A(csTime));
		if (nTime < 500 || nTime > 30000)
		{
			MessageBoxW(_T("定时时间长度范围(500~30000)!"), _T("警告"), MB_OK | MB_ICONWARNING);
			m_cSerialSendTimerCbx.SetCheck(FALSE);
			return;
		}

		m_cSerialSendTimerValueEdt.EnableWindow(FALSE);
		::SetTimer(m_hWnd, WM_USER_TIMER_ONTIMERSENDDATA, nTime, NULL);
	}
	else
	{
		m_cSerialSendTimerValueEdt.EnableWindow(TRUE);
		::KillTimer(m_hWnd, WM_USER_TIMER_ONTIMERSENDDATA);
	}

}

// CSerialPortDlg 刷新串口
void CSerialPortDlg::OnBnClickedButtonSerialRefresh()
{
	m_cSerialPort.EnumSerialPort();		// 枚举串口

	// 添加串口
	m_cSerialPortNameCbx.ResetContent();	// 清空Cbx
	for (auto iter = m_cSerialPort.m_mapEnumCOM.begin(); iter != m_cSerialPort.m_mapEnumCOM.end(); ++iter)
	{
		USES_CONVERSION;
		m_cSerialPortNameCbx.InsertString(iter->first, A2T((iter->second).c_str()));
	}

	m_cSerialPortNameCbx.SetCurSel(0);
}

// CSerialPortDlg 打开串口
void CSerialPortDlg::OnBnClickedButtonSerialOpen()
{
	// 串口打开状态
	if (!m_cSerialPort.CCSerialPortBaseGetStatus())
	{
		// 打开串口
		if (!SerialPortConfigCheck())
		{
			return;
		}

		bool bRet = false;
		S_SERIALPORT_PROPERTY sSerialPortProperty = { 0 };

		// 填充串口参数
		SerialPortSetValue(&sSerialPortProperty);

		// 打开串口函数
		bRet = m_cSerialPort.CCSerialPortBaseOpenPort(sSerialPortProperty);
		if (!bRet)
		{
			// 打开失败
			MessageBoxW(_T("串口打开失败"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}

		// 开启串口收发线程
		//SerialPortStartTransThread();
		SerialPortStartRecvThread();

		// 窗口显示函数
		SerialPortSetOpenStatus();

		// 开启接收计数定时器
		::SetTimer(m_hWnd, WM_USER_TIMER_ONREFRESHSPEED, 100, NULL);
	}
	else
	{
		// 关闭接收计数定时器
		::KillTimer(m_hWnd, WM_USER_TIMER_ONREFRESHSPEED);

		// 关闭串口
		m_cSerialPort.CCSerialPortBaseClosePort();

		// 关闭串口收发线程
		//SerialPortCloseTransThread();
		SerialPortCloseRecvThread();

		// 清除串口接收速度
		SerialPortClearRecvSpeed();

		// 窗口显示函数
		SerialPortSetCloseStatus();
	}

}

// CSerialPortDlg 清接收区
void CSerialPortDlg::OnBnClickedButtonSerialClearReceive()
{
	m_cSerialRecvEdt.SetWindowTextW(_T(""));

	m_dwRecvCount = 0;
	m_cSerialInfoAllRecvStic.SetWindowTextW(_T("已接收:0"));
	m_cSerialInfoAllRecvStic.Invalidate(FALSE);
	m_cSerialInfoAllRecvStic.UpdateWindow();
}

// CSerialPortDlg 清发送区
void CSerialPortDlg::OnBnClickedButtonSerialClearSend()
{
	m_cSerialSendEdt.SetWindowTextW(_T(""));

	m_dwTranCount = 0;
	m_cSerialInfoAllSendStic.SetWindowTextW(_T("已发送:0"));
	m_cSerialInfoAllSendStic.Invalidate(FALSE);
	m_cSerialInfoAllSendStic.UpdateWindow();
}

// CSerialPortDlg 发送数据
void CSerialPortDlg::OnBnClickedButtonSerialSend()
{
	if (NULL != m_hTran)
	{
		::WaitForSingleObject(m_hTran, INFINITE);
		::CloseHandle(m_hTran);
		m_hTran = NULL;
	}

	SerialPortStartTransThread();
}
