#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FloodFill");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hRed, hBlue, hGreen,hYellow, hOldBrush;
	HPEN hPen;

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hPen=CreatePen(PS_SOLID,5,RGB(0,0,0));
		SelectObject(hdc,hPen);
		hRed=CreateSolidBrush(RGB(255,0,0));
		hGreen=CreateSolidBrush(RGB(0,255,0));
		hBlue=CreateSolidBrush(RGB(0,0,255));
		SelectObject(hdc,hRed);
		Rectangle(hdc,10,10,200,200);

		SelectObject(hdc,hGreen);
		Ellipse(hdc,100,50,300,250);

		SelectObject(hdc,GetStockObject(NULL_PEN));
		SelectObject(hdc,hBlue);
		Ellipse(hdc,200,50,400,250);
		SelectObject(hdc,GetStockObject(GRAY_BRUSH));
		Ellipse(hdc,30,30,100,100);

		DeleteObject(hBlue);
		DeleteObject(hGreen);
		DeleteObject(hRed);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
		return 0;
	// 사각형을 노란색으로 채우기
	case WM_LBUTTONDOWN:
		hdc=GetDC(hWnd);
		hYellow=CreateSolidBrush(RGB(255,255,0));
		hOldBrush=(HBRUSH)SelectObject(hdc,hYellow);

		ExtFloodFill(hdc,20,20,RGB(0,0,0),FLOODFILLBORDER);
		//ExtFloodFill(hdc,20,20,RGB(255,0,0),FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc,hOldBrush));
		ReleaseDC(hWnd,hdc);
		return 0;
	// 가운데 원을 노란색으로 채우기
	case WM_RBUTTONDOWN:
		hdc=GetDC(hWnd);
		hYellow=CreateSolidBrush(RGB(255,255,0));
		hOldBrush=(HBRUSH)SelectObject(hdc,hYellow);

		ExtFloodFill(hdc,150,150,RGB(0,255,0),FLOODFILLSURFACE);
		//ExtFloodFill(hdc,150,150,RGB(0,0,0),FLOODFILLBORDER);

		DeleteObject(SelectObject(hdc,hOldBrush));
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

