#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Gradient");

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
	RECT crt;
	TRIVERTEX vert[4];
	GRADIENT_RECT grt;
	GRADIENT_TRIANGLE gtr;

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd,&crt);

//* 사각형으로 칠하기
		vert[0].x=vert[0].y=0;
		vert[0].Red=vert[0].Green=vert[0].Blue=vert[0].Alpha=0;
		vert[1].x=crt.right;vert[1].y=crt.bottom;
		vert[1].Red=vert[1].Green=vert[1].Alpha=0;vert[1].Blue=0xff00;
		grt.UpperLeft=0;
		grt.LowerRight=1;
		GradientFill(hdc,vert,2,&grt,1,GRADIENT_FILL_RECT_H);
//*/
/* 삼각형으로 칠하기
		vert[0].x=crt.right/2;vert[0].y=0;
		vert[0].Red=0;vert[0].Green=0xff00;vert[0].Blue=0;vert[0].Alpha=0;
		vert[1].x=0;vert[1].y=crt.bottom;
		vert[1].Red=0;vert[1].Green=0;vert[1].Alpha=0;vert[1].Blue=0xff00;
		vert[2].x=crt.right;vert[2].y=crt.bottom;
		vert[2].Red=0xff00;vert[2].Green=0;vert[2].Alpha=0;vert[2].Blue=0;
		gtr.Vertex1=0;
		gtr.Vertex2=1;
		gtr.Vertex3=2;
		GradientFill(hdc,vert,3,&gtr,1,GRADIENT_FILL_TRIANGLE);
//*/

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

