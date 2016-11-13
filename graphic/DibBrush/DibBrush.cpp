#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DibBrush");

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
	WndClass.style=0;
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
HBRUSH DibBrush;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;
	HRSRC hRes;
	HGLOBAL hData,hLock;
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";
	HANDLE hFile;
	DWORD FileSize, dwRead;
	PBYTE BmpFile;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hRes=FindResource(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1),RT_BITMAP);
		hData=LoadResource(g_hInst,hRes);
		hLock=LockResource(hData);
		DibBrush=CreateDIBPatternBrushPt(hLock,DIB_RGB_COLORS);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd,&crt);
		FillRect(hdc,&crt,DibBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner=hWnd;
		OFN.lpstrFilter="Bitmap File(*.bmp)\0*.bmp\0";
		OFN.lpstrFile=lpstrFile;
		OFN.nMaxFile=MAX_PATH;
		if (GetOpenFileName(&OFN)!=0) {
			hFile=CreateFile(lpstrFile,GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile==INVALID_HANDLE_VALUE) {
				return 0;
			}

			FileSize=GetFileSize(hFile,NULL);
			BmpFile=(PBYTE)malloc(FileSize);
			ReadFile(hFile,BmpFile,FileSize,&dwRead,NULL);
			CloseHandle(hFile);
			if (DibBrush) {
				DeleteObject(DibBrush);
			}
			DibBrush=CreateDIBPatternBrushPt(BmpFile+sizeof(BITMAPFILEHEADER),
				DIB_RGB_COLORS);
			free(BmpFile);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		DeleteObject(DibBrush);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
