#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ClipRgn");

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
	HRGN hRgn1, hRgn2, hRgn3;
	TCHAR *str="�츮�� ���� ������ ������ ����� ��� �� ���� �¾��."
		"������ ���� ���� ���ÿ� �ǻ�� ������ ���ֵ����� �ڼ��� Ȯ��"
		"�ϰ� ������ �η� ������ �̹����� ����. �̿� �츮�� ���ư� �ٸ�"
		"���� ������ ��ǥ�� ��´�. ������ ������ ưư�� ������ Ÿ��"
		"�������� ������ �����ϰ� ��¼�� ��¼�� ���� ���� �����̾����ϴ�"
		"���� �ڶ����� �±ر�տ� ������ ������ ������ ������ ���Ͽ� ����"
		"������ ���� �漺�� ���� ���� ���� �����մϴ�.";
	RECT rt={10,10,250,220};
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hRgn1=CreateEllipticRgn(10,10,200,150);
		hRgn2=CreateRectRgn(70,70,250,220);
		hRgn3=CreateRectRgn(0,0,0,0);
		CombineRgn(hRgn3, hRgn1, hRgn2, RGN_OR);

		SelectClipRgn(hdc,hRgn3);
		DrawText(hdc,str,-1,&rt,DT_WORDBREAK);

		DeleteObject(hRgn1);
		DeleteObject(hRgn2);
		DeleteObject(hRgn3);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

