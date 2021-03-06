// CAboutExDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CDA.h"
#include "CAboutExDlg.h"
#include "afxdialogex.h"
#include "ResourceEx.h"

// CAboutExDlg 对话框

IMPLEMENT_DYNAMIC(CAboutExDlg, CDialogEx)

CAboutExDlg::CAboutExDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUT_DIALOG, pParent)
{
	ConstructionExtra();
}

CAboutExDlg::~CAboutExDlg()
{
	m_Brush.DeleteObject();
}

// CAboutExDlg 构造函数
void CAboutExDlg::ConstructionExtra()
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
}

// CAboutExDlg 初始化窗口形状
void CAboutExDlg::InitWindowSharp()
{
	GetClientRect(&m_cWindowRect);
}

// CAboutExDlg 重绘窗口
void CAboutExDlg::RePaintWindow(CDC & dc)
{
	CDC MemDC;
	CBitmap MemBitmap;
	CRect MemRect;

	GetClientRect(&MemRect);

	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, MemRect.Width(), MemRect.Height());

	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(&MemRect, RGB(255, 255, 255));

	ReDrawImage(MemDC, (void*)g_ucAuthorInfo, sizeof(g_ucAuthorInfo), (MemRect.Width() - 640) / 2, (MemRect.Height() - 240) / 2);
	ReDrawImage(MemDC, (void*)g_ucLovelyCat, sizeof(g_ucLovelyCat), MemRect.right - 72, MemRect.bottom - 72);

	dc.BitBlt(m_cWindowRect.left, m_cWindowRect.top, m_cWindowRect.Width(), m_cWindowRect.Height(), &MemDC, MemRect.left, MemRect.top, SRCCOPY);

	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

// CAboutExDlg 加载内存图像
void CAboutExDlg::LoadMemImage(CImage & Image, void * pMemData, long nSize)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	void *  pData = GlobalLock(hGlobal);
	memcpy(pData, pMemData, nSize);
	GlobalUnlock(hGlobal);

	IStream *  pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		Image.Load(pStream);
		pStream->Release();
	}
	GlobalFree(hGlobal);
}

// CAboutExDlg 重绘图像
void CAboutExDlg::ReDrawImage(CDC & MemDC, void * pData, long nSize, int X, int Y)
{
	CImage Image;

	LoadMemImage(Image, pData, nSize);

	if (Image.GetBPP() == 32)//包含Alpha通道
	{
		for (int i = 0; i < Image.GetWidth(); i++)
		{
			for (int j = 0; j < Image.GetHeight(); j++)
			{
				byte *pByte = (byte *)Image.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	Image.Draw(MemDC, X, Y);
	Image.Destroy();
}

void CAboutExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutExDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAboutExDlg 消息处理程序


BOOL CAboutExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWindowSharp();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAboutExDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RePaintWindow(dc);
}


HBRUSH CAboutExDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
