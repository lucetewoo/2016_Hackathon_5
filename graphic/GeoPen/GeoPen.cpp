#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GeoPen");

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
	HPEN hGeo, hOldPen;
	LOGBRUSH logbrush;
	DWORD Style[]={5,5,4,4,3,3,2,2,1,1};
	
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);

//* ÆÄ¶õ»öÀÇ ±½Àº Ææ
		logbrush.lbStyle=BS_HATCHED;
		logbrush.lbColor=RGB(0,0,255);
		logbrush.lbHatch=HS_DIAGCROSS;
		hGeo=ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | 
			PS_JOIN_ROUND,20,&logbrush,0,NULL);

//*/
/* »ç¿ëÀÚ Á¤ÀÇ Ææ
		logbrush.lbStyle=BS_SOLID;
		logbrush.lbColor=RGB(255,0,0);
		logbrush.lbHatch=0;
		hGeo=ExtCreatePen(PS_GEOMETRIC | PS_USERSTYLE,
			1,&logbrush,10,Style);
//*/
		hOldPen=(HPEN)SelectObject(hdc, hGeo);
		MoveToEx(hdc,50,50,NULL);
		LineTo(hdc,200,50);
		LineTo(hdc,200,200);
		SelectObject(hdc, hOldPen);
		DeleteObject(hGeo);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

