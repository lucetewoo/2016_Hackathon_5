#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ShootBullet");

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

#define R 7
int tx,ty;
POINT pts[2048];
int num,idx;
void CALLBACK LineDDAProc(int x, int y, LPARAM lpData)
{
	pts[num].x=x;
	pts[num].y=y;
	num++;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		tx=300;
		ty=200;
		return 0;
	case WM_LBUTTONDOWN:
		num=0;
		LineDDA(tx,ty,LOWORD(lParam),HIWORD(lParam),LineDDAProc,NULL);
		idx=0;
		SetTimer(hWnd,1,10,NULL);
		return 0;
	case WM_RBUTTONDOWN:
		tx=LOWORD(lParam);
		ty=HIWORD(lParam);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_TIMER:
		if (idx < num) {
			hdc=GetDC(hWnd);
			SelectObject(hdc,GetStockObject(GRAY_BRUSH));
			Ellipse(hdc,pts[idx].x-R,pts[idx].y-R,pts[idx].x+R,pts[idx].y+R);
			ReleaseDC(hWnd,hdc);
			idx+=30;
		} else {
			idx=-1;
			KillTimer(hWnd,1);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetTextAlign(hdc,TA_CENTER);
		TextOut(hdc,tx,ty,"Tank",4);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
