#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MkPath");

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
	PAINTSTRUCT ps;
	HDC hdc;
	HFONT font,oldfont;
	int i;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		font=CreateFont(100,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"궁서");
		oldfont=(HFONT)SelectObject(hdc,font);

		BeginPath(hdc);
		TextOut(hdc,50,50,"KOREA",5);
		EndPath(hdc);

		// 패스의 외곽선 그리기
		StrokePath(hdc);
		
		// 패스 내부를 채우기
		//SelectObject(hdc,(HBRUSH)GetStockObject(GRAY_BRUSH));
		//FillPath(hdc);

		// 패스의 외곽선, 내부 모두 그리기
		//SelectObject(hdc,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
		//StrokeAndFillPath(hdc);
		
/*		// 패스를 클리핑 영역으로 만들기
		SelectClipPath(hdc, RGN_COPY);
		SelectObject(hdc,(HBRUSH)GetStockObject(NULL_BRUSH));
		for (i=0;i<200;i+=3) {
			Ellipse(hdc,200-i,100-i,200+i,100+i);
		}
//*/

		SelectObject(hdc,oldfont);
		DeleteObject(font);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

