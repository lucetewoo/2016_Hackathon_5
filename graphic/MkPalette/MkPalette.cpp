#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MkPalette");

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
	HBRUSH hBrush, hOldBrush;
	static LOGPALETTE *pPal;
	static HPALETTE hPal;
	static int nFree;
	int i;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
		hdc=GetDC(hWnd);
		if ((GetDeviceCaps(hdc,RASTERCAPS) & RC_PALETTE)==0) {
			MessageBox(hWnd,"현재 화면 모드는 팔레트를 지원하지 않습니다",
				"알림",MB_OK);
			return -1;
		}
		nFree=GetDeviceCaps(hdc,SIZEPALETTE)-GetDeviceCaps(hdc,NUMRESERVED);
		ReleaseDC(hWnd,hdc);
		pPal=(LOGPALETTE *)GlobalAlloc(GPTR,sizeof(LOGPALETTE)+
			nFree*sizeof(PALETTEENTRY));
		pPal->palVersion=0x300;
		pPal->palNumEntries=nFree;
		for (i=0;i<nFree;i++) {
			pPal->palPalEntry[i].peRed=0;
			pPal->palPalEntry[i].peGreen=0;
			pPal->palPalEntry[i].peBlue=i;
			pPal->palPalEntry[i].peFlags=NULL;
		}
		hPal=CreatePalette(pPal);
		return 0;
	case WM_QUERYNEWPALETTE:
		if (hPal==NULL)
			return FALSE;
		hdc=GetDC(hWnd);
		SelectPalette(hdc,hPal,FALSE);
		RealizePalette(hdc);
		ReleaseDC(hWnd,hdc);
		InvalidateRect(hWnd,NULL,TRUE);
		return TRUE;
	case WM_PALETTECHANGED:
		if ((hPal==NULL) && ((HWND)wParam==hWnd))
			break;
		hdc=GetDC(hWnd);
		SelectPalette(hdc,hPal,FALSE);
		RealizePalette(hdc);
		UpdateColors(hdc);
		ReleaseDC(hWnd,hdc);
		return TRUE;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SelectPalette(hdc,hPal,FALSE);
		RealizePalette(hdc);
		for (i=0;i<nFree;i++) {
			hBrush=CreateSolidBrush(PALETTEINDEX(i));
			hOldBrush=(HBRUSH)SelectObject(hdc,hBrush);
			SetRect(&rt,i*3,0,i*3+3,600);
			FillRect(hdc,&rt,hBrush);
			SelectObject(hdc,hOldBrush);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hPal);
		GlobalFree((HGLOBAL)pPal);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

