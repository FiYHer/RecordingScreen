#pragma once

#include <windows.h>

/*
屏幕捕获与播放类
*/

class RecordingScreen
{
private:
	//主窗口句柄
	HWND m_hWnd;

	//位图缓冲区
	LPBYTE m_pScreen;

	//位图基本信息
	BITMAPINFO m_stBitInfo;

	//桌面的宽度和高度
	INT m_nWidth;
	INT m_nHeight;
public:
	RecordingScreen();
	~RecordingScreen();
public:
	/*
	获取屏幕位图 [获取返回的BYTE的长度就是宽度乘以高度乘以3]
	*@pWidth:宽度
	*@pHieght:高度
	*/
	BYTE* GetScreenBuffer(INT* pWidth = NULL, INT* pHieght = NULL);

	/*
	设置窗口的句柄
	*@hWnd:窗口句柄
	*/
	VOID SetWindowhWnd(HWND hWnd);

	/*
	绘制函数
	*@xBegin:开始绘制的x轴
	*@yBegin:开始绘制的y轴
	*@nBitmapwidth:图片宽度
	*@nBitmapheight:图片高度
	*@nToWidth:缩放成多宽
	*@nToHeight:缩放成多高
	*/
	VOID DrawScreen(CONST LPBYTE lpBuffer,
		INT xBegin = NULL, INT yBegin = NULL,
		INT nBitmapwidth = NULL, INT nBitmapheight = NULL,
		INT nToWidth = NULL, INT nToHeight = NULL);

	VOID DrawScreen(INT xBegin = NULL, INT yBegin = NULL,
		INT nToWidth = NULL, INT nToHeight = NULL);
};

