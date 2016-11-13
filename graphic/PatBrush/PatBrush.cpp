#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PatBrush");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include "resource.h"
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hMyBrush, hOldBrush;
	static HBITMAP bit;
	switch (iMessage) {
	case WM_CREATE:
		bit=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hMyBrush=CreatePatternBrush(bit);
		hOldBrush=(HBRUSH)SelectObject(hdc,hMyBrush);
/* 흑백 비트맵의 색상 변경
		SetTextColor(hdc,RGB(0,0,255));
		SetBkColor(hdc,RGB(255,255,0));
//*/
		Ellipse(hdc,50,50,200,200);

/* 브러시 원점 테스트
		Rectangle(hdc,40,40,300,100);
		SetBrushOrgEx(hdc,2,2,NULL);
		Rectangle(hdc,42,122,300,180);
//*/
		SelectObject(hdc,hOldBrush);
		DeleteObject(hMyBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(bit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

