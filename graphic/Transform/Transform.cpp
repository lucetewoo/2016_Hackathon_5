#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Transform");

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

#include <math.h>
#define DEGREE 45
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	XFORM XForm={1,0,0,1,0,0};		// 디폴트 변환
	//XFORM XForm={1,0,0,1,50,50};	// 이동
	//XFORM XForm={2,0,0,2,0,0};	// 확대
	//XFORM XForm={1,0,1,1,0,0};	// 기울이기
	//XFORM XForm={1,0,0,-1,0,200};	// 뒤집기
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetGraphicsMode(hdc,GM_ADVANCED);

/* 회전
		XForm.eM11=(float)cos(DEGREE*3.14/180);
		XForm.eM12=(float)sin(DEGREE*3.14/180);
		XForm.eM21=(float)-sin(DEGREE*3.14/180);
		XForm.eM22=(float)cos(DEGREE*3.14/180);
		XForm.eDx=100;
		XForm.eDy=0;
//*/
		SetWorldTransform(hdc,&XForm);

		// 사람 얼굴 모양을 그린다.
		Ellipse(hdc,10,10,150,150);
		Ellipse(hdc,40,35,65,60);
		Ellipse(hdc,95,35,120,60);
		Rectangle(hdc,70,70,90,90);
		Rectangle(hdc,40,110,120,120);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
