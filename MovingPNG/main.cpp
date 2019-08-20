#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include <gdiplusgraphics.h>
#include <GamePad.h>
#include "Gamepad.h"
#include "memory"
#include <Mmsystem.h>;


#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);
JOYINFO joyinfo;
UINT wNumDevs, wDeviceID;
BOOL bDev1Attached, bDev2Attached;
std::unique_ptr<DirectX::GamePad> gamepad;


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	gamepad = std::make_unique<DirectX::GamePad>();
	GdiplusStartupInput gdiplusStartupToken;
	ULONG_PTR gdiplusToken;
	Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupToken, NULL);
	WNDCLASSEX wcex; HWND hWnd; MSG msg;


	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "HelloWorldClass";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);

	hWnd = CreateWindow("HelloWorldClass", "Hello, World!",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	//DirectX::GamePad;


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;

}
int speed = 10, winY, winX;
int mousewheel;
int speedX = 3, speedY = 3;
int left = 0, top = 0, right = 10, bottom = 10;
int width = 0, height = 0, x, y;
boolean isShift = false;
BOOLEAN passiveMovingFlag = false;
int flag = 1;




void DrawImage(HDC hdc, HWND hWnd, int x, int y)
{
	Gdiplus::Bitmap bitmap(L"C:\\Users\\eugen\\OneDrive\\Рабочий стол\\WinAPI\\MovingPNG\\pineapple.png");
	width = bitmap.GetWidth();
	height = bitmap.GetHeight();
	HBITMAP hBmp;
	bitmap.GetHBITMAP(Gdiplus::Color(255, 255, 255), &hBmp);
	HDC hDcBitmap = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = (HBITMAP)SelectObject(hDcBitmap, hBmp);
	BitBlt(hdc, x, y, width, height, hDcBitmap, 0, 0, SRCCOPY);
	SelectObject(hDcBitmap, hBitmap);
	DeleteObject(hBmp);
	DeleteDC(hDcBitmap);
	ReleaseDC(0, hdc);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	
	HDC hdc = 0;
	Graphics graphics(hdc);
	//Image image(L"pineapple.png");
	RECT rect;

	MINMAXINFO *pt;
	POINT ptMin;
	MINMAXINFO *pInfo;

	switch (message)
	{
	case WM_SIZE:
		winX = LOWORD(lParam);
		winY = HIWORD(lParam);
		if ((winX < left + width) or (left < 0)) {
			left = winX - width;
			DrawImage(hdc, hWnd, left, top);
			InvalidateRect(hWnd, NULL, true);
		}
		if ((winY < top + height) or (top < 0)) {
			top = winY - height;
			DrawImage(hdc, hWnd, left, top);
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_GETMINMAXINFO:
		pInfo = (MINMAXINFO *)lParam;
		ptMin = { width + 16, height + 40 };
		pInfo->ptMinTrackSize = ptMin;
		return 0;
		break;
	//case WM_GETMINMAXINFO:
	//pt =(MINMAXINFO*) lParam;
	//pt->ptMinTrackSize.x = 150;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//Rectangle(hdc, left, top, right, bottom);
		DrawImage(hdc, hWnd, left, top);
		GetClientRect(hWnd, &rect);
		EndPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		break;
	case WM_KEYDOWN:
		//SetTimer(hWnd, 1, 1000, NULL);
		//passiveMovingFlag = false;
		switch (wParam)
		{
		case VK_RIGHT:
			if (winX > left + width) 
				left += flag*speed; 
			else {
				left -= 20;
				flag = -1;
			}
			break;
		case VK_LEFT:
			if (left >= 0)
				left -= flag*speed;
			else {
				left += 20;
				flag = -1;
			}
			break;
		case VK_DOWN:
			if (winY > top + height) {
				top += flag*speed;
				
			}
			else {
				top -= 20;
				flag = -1;
			}
			break;
		case VK_UP:
			if (top > 0) {
				top -= flag*speed;
			}
			else {
				top += 20;
				flag = -1;
			}
			break;
		case VK_SHIFT:
			isShift = true;
			break;
		}
		SetTimer(hWnd, 2, 200, NULL);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MOUSEWHEEL:
		mousewheel = GET_WHEEL_DELTA_WPARAM(wParam);
		if (!isShift) {
			if (mousewheel < 0)
			{
				if (winY > top + height)
					top += speed;
				else
					top -= 20;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			if (mousewheel > 0)
			{
				if (top > 0)
					top -= speed;
				else
					top += 20;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
		}
		else 
		{
			if (mousewheel < 0)
			{
				if (winX > left + width)
					left += speed;
				else
					left -= 20;
				break;
			}
			if (mousewheel > 0)
			{
				if (left >= 0)
					left -= speed;
				else
					left += 20;
				break;
			}
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_TIMER:
	
		switch (wParam)
		{
		case 2:
		
			flag = 1;
			KillTimer(hWnd, 2);
			break;
		

		}
		break;
	case WM_LBUTTONDBLCLK:
		//MessageBox(hWnd, "Hello, World!", "Message", MB_OK);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		
		
		auto state = gamepad->GetState(0);
		
		if (state.IsConnected())
		{
			if (state.IsAPressed())
			{
				MessageBox(hWnd, "Hello, World!", "Message", MB_OK);
				
			}

			if (state.IsLeftThumbStickRight() || state.IsLeftThumbStickLeft())
			{
				
				MessageBox(hWnd, "Hello, World!", "Message", MB_OK);
			}
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			MessageBox(hWnd, "Hello, World!", "Message", MB_OK);
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			MessageBox(hWnd, "Hello, World!", "Message", MB_OK);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			MessageBox(hWnd, "Hello, World!", "Message", MB_OK);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
