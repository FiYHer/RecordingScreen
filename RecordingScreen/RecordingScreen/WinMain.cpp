
#include "RecordingScreen.h"
#include <process.h>

RecordingScreen MyTest;

UINT CALLBACK Thread(LPVOID lpData)
{
	HWND hWnd = (HWND)lpData;
	INT nBitmapwidth = NULL;
	INT nBitmapheight = NULL;
	RECT stRect = { 0 };
	LPBYTE pBuffer = NULL;
	while (true)
	{
		GetClientRect(hWnd, &stRect);

		//1.
		pBuffer = MyTest.GetScreenBuffer(&nBitmapwidth, &nBitmapheight);
		MyTest.DrawScreen(pBuffer, 0, 0, nBitmapwidth, nBitmapheight,
			stRect.right - stRect.left, stRect.bottom - stRect.top);
		
		//2.
		MyTest.DrawScreen(0, 0, stRect.right - stRect.left, stRect.bottom - stRect.top);
	}
	return 0;
}

LRESULT CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDc = NULL;
	HDC DisplayDC = NULL;
	PAINTSTRUCT stPs = { 0 };
	RECT stRect = { 0 };
	switch (uMsg)
	{
	case WM_CREATE:
		_beginthreadex(0, 0, Thread, hWnd, 0, 0);
		break;
	case WM_PAINT:
		hDc = BeginPaint(hWnd, &stPs);
		EndPaint(hWnd, &stPs);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
	return 1;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	HWND hWnd = NULL;
	WNDCLASSEX stWindow = { 0 };
	MSG stMsg = { 0 };

	stWindow.cbSize = sizeof(WNDCLASSEX);
	stWindow.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	stWindow.hCursor = LoadCursor(NULL, IDC_ARROW);
	stWindow.hInstance = hInstance;
	stWindow.lpfnWndProc = Proc;
	stWindow.lpszClassName = "RecordingScreen";
	stWindow.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClassEx(&stWindow))
	{
		return -1;
	}

	hWnd = CreateWindowEx(NULL, "RecordingScreen", "RecordingScreen",
		WS_OVERLAPPEDWINDOW,100, 100, 500, 500,
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return -1;
	}

	MyTest.SetWindowhWnd(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&stMsg,0,0,0))
	{
		TranslateMessage(&stMsg);
		DispatchMessage(&stMsg);
	}
	return stMsg.wParam;
}
