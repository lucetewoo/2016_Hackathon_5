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
	TCHAR *str="우리는 민족 중흥의 역사적 사명을 띠고 이 땅에 태어났다."
		"조상의 빛난 얼을 오늘에 되살려 안으로 자주독립의 자세를 확립"
		"하고 밖으로 인류 공영에 이바지할 때다. 이에 우리의 나아갈 바를"
		"밝혀 교육의 지표로 삼는다. 성실한 마음과 튼튼한 몸으로 타고난"
		"저마다의 소질을 개발하고 어쩌고 저쩌고 국민 교육 헌장이었습니다"
		"나는 자랑스런 태극기앞에 조국과 민족의 무궁한 영광을 위하여 몸과"
		"마음을 바쳐 충성을 다할 것을 굳게 다짐합니다.";
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

