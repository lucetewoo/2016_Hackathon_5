#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ReadDIB2");

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


BITMAPFILEHEADER *fh=NULL;
BITMAPINFOHEADER *ih;
int bx,by;
BYTE *pRaster;
HPALETTE hPal;
void LoadDIB(LPCTSTR Path)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	int nCol,i;
	LOGPALETTE *pPal;
	RGBQUAD *pRgb;

	hFile=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return;
	}

	FileSize=GetFileSize(hFile,NULL);

	if (fh)
		free(fh);
	fh=(BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile,fh,FileSize,&dwRead,NULL);
	CloseHandle(hFile);

	pRaster=(PBYTE)fh+fh->bfOffBits;
	ih=(BITMAPINFOHEADER *)((PBYTE)fh+sizeof(BITMAPFILEHEADER));
	bx=ih->biWidth;
	by=ih->biHeight;

	// 팔레트가 이미 정의되어 있으면 해제한다.
	if (hPal) {
		DeleteObject(hPal);
	}

	// 비트맵의 색상수를 구한다.
	nCol=(ih->biBitCount==24 ? 0:1 << ih->biBitCount);
	// 트루컬러인 경우는 팔레트가 없다.
	if (nCol != 256) {
		hPal=NULL;
	} else {
		// 비트맵의 색상 테이블 포인터를 구한다.
		pRgb=(RGBQUAD *)((PBYTE)ih+sizeof(ih));

		// 팔레트 생성을 위한 메모리를 할당한다.
		pPal=(LOGPALETTE *)malloc(sizeof(LOGPALETTE)+nCol*sizeof(PALETTEENTRY));
		pPal->palNumEntries=nCol;
		pPal->palVersion=0x300;

		// 비트맵의 색상 정보를 팔레트로 읽어들인다.
		for (i=0;i<nCol;i++) {
			pPal->palPalEntry[i].peRed=pRgb[i].rgbRed;
			pPal->palPalEntry[i].peGreen=pRgb[i].rgbGreen;
			pPal->palPalEntry[i].peBlue=pRgb[i].rgbBlue;
			pPal->palPalEntry[i].peFlags=0;
		}

		// 팔레트를 생성한다.
		hPal=CreatePalette(pPal);
		free(pPal);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner=hWnd;
		OFN.lpstrFilter="Bitmap File(*.bmp)\0*.bmp\0";
		OFN.lpstrFile=lpstrFile;
		OFN.nMaxFile=256;
		if (GetOpenFileName(&OFN)!=0) {
			LoadDIB(lpstrFile);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		if (fh) {
			// 팔레트가 정의되어 있으면 선택해 준다.
			if (hPal && (GetDeviceCaps(hdc,RASTERCAPS) & RC_PALETTE)) {
				SelectPalette(hdc,hPal,FALSE);
				RealizePalette(hdc);
			}
			SetDIBitsToDevice(hdc,0,0,bx,by,0,0,0,by,pRaster,
				(BITMAPINFO *)ih,DIB_RGB_COLORS);
		}
		EndPaint(hWnd, &ps);
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
	case WM_DESTROY:
		if (fh)
			free(fh);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

