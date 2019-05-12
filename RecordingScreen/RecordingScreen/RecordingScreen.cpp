#include "RecordingScreen.h"



RecordingScreen::RecordingScreen()
{
	m_hWnd = NULL;
	m_pScreen = nullptr;
	m_nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	m_nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	ZeroMemory(&m_stBitInfo, sizeof(BITMAPINFO));
	m_stBitInfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	m_stBitInfo.bmiHeader.biWidth = m_nWidth;
	m_stBitInfo.bmiHeader.biHeight = m_nHeight;
	m_stBitInfo.bmiHeader.biPlanes = 1;
	m_stBitInfo.bmiHeader.biBitCount = 24;
}


RecordingScreen::~RecordingScreen()
{
	if (m_pScreen)
	{
		delete[] m_pScreen;
	}
}

BYTE* RecordingScreen::GetScreenBuffer(INT* pWidth, INT* pHieght)
{
	HDC hDc = NULL;
	HDC hDcMem = NULL;
	HBITMAP hBit = NULL;
	LPBYTE pBuffer = NULL;

	do 
	{
		//获取桌面DC
		hDc = CreateDC("DISPLAY", NULL, NULL, NULL);
		if (!hDc)
		{
			break;
		}

		*pWidth = m_nWidth;
		*pHieght = m_nHeight;

		//位图的大小
		if (!m_pScreen)
		{
			m_pScreen = new BYTE[m_nWidth * m_nHeight * 3];
			if (!m_pScreen)
			{
				break;
			}
			memset(m_pScreen, 0, sizeof(BYTE)*(m_nWidth * m_nHeight * 3));
		}

		//创建兼容DC
		hDcMem = CreateCompatibleDC(NULL);
		if (!hDcMem)
		{
			break;
		}

		hBit = CreateDIBSection(hDcMem,&m_stBitInfo,
			DIB_RGB_COLORS, (VOID**)(&pBuffer), NULL, NULL);
		SelectObject(hDcMem, hBit);

		StretchBlt(hDcMem, NULL, NULL,
			m_nWidth, m_nHeight, hDc, NULL, NULL,
			m_nWidth, m_nHeight, SRCCOPY);

		CopyMemory(m_pScreen, pBuffer, sizeof(BYTE)*(m_nWidth * m_nHeight * 3));
	} while (FALSE);
	if (hDc)
	{
		DeleteDC(hDc);
	}
	if (hDcMem)
	{
		DeleteObject(hDcMem);
	}
	if (hBit)
	{
		DeleteObject(hBit);
	}
	return m_pScreen;
}

VOID RecordingScreen::SetWindowhWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

VOID RecordingScreen::DrawScreen(CONST LPBYTE lpBuffer, 
	INT xBegin /*= NULL*/, 
	INT yBegin /*= NULL*/, 
	INT nBitmapwidth /*= NULL*/, 
	INT nBitmapheight /*= NULL*/, 
	INT nToWidth , 
	INT nToHeight)
{
	//获取窗口的HDC
	HDC hDc = GetDC(m_hWnd);
	if (!hDc)
	{
		return;
	}

	//默认图片的大小为当前窗口的大小
	if (!nBitmapwidth || !nBitmapheight)
	{
		nBitmapwidth = m_nWidth;
		nBitmapheight = m_nHeight;
	}

	//默认图片不进行缩放
	if (!nToWidth || !nToHeight)
	{
		nToWidth = m_nWidth;
		nToHeight = m_nHeight;
	}

	//设置模式，不然图片缩放失真严重
	// COLORONCOLOR 和 HALFTONE 都不会导致图片缩小的失真
	SetStretchBltMode(hDc, HALFTONE);

	//第四个和第五个就是图片缩放的宽度和高度
	StretchDIBits(hDc, xBegin, yBegin, nToWidth, nToHeight, NULL, NULL,
		nBitmapwidth, nBitmapheight, lpBuffer, &m_stBitInfo, DIB_RGB_COLORS, SRCCOPY);

	if (hDc)
	{
		ReleaseDC(m_hWnd, hDc);
	}
}

VOID RecordingScreen::DrawScreen(INT xBegin , 
	INT yBegin , 
	INT nToWidth , 
	INT nToHeight )
{
	HDC hDcScreen = NULL;
	HDC hDc = NULL;

	//获取当前桌面的HDC
	hDcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	if (!hDcScreen)
	{
		return;
	}

	//获取当前窗口的HDC
	hDc = GetDC(m_hWnd);
	if(!hDc)
	{
		DeleteDC(hDcScreen);
		return;
	}

	//默认图片不缩放
	if (!nToWidth || !nToHeight)
	{
		nToWidth = m_nWidth;
		nToHeight = m_nHeight;
	}

	//设置图片的绘制模式防止失真
	SetStretchBltMode(hDc, HALFTONE);

	//把桌面DC绘制到窗口DC上 图片缩放是第四个和第五个参数
	StretchBlt(hDc, xBegin, yBegin, nToWidth, nToHeight,
		hDcScreen, 0, 0, m_nWidth, m_nHeight, SRCCOPY);

	if (hDc)
	{
		ReleaseDC(m_hWnd, hDc);
	}
	if (hDcScreen)
	{
		DeleteDC(hDcScreen);
	}
}
