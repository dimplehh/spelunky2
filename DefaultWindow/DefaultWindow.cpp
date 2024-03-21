// DefaultWindow.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DefaultWindow.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND	g_hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			// HINSTANCE : 현재 프로그램의 고유 식별 번호 (Instance : 메모리에 할당되는 실체)
                     _In_opt_ HINSTANCE hPrevInstance,	// 전에 실행되었던 인스턴스의 핸들(없을 경우 NULL)
                     _In_ LPWSTR    lpCmdLine,			// LPWSTR : 유니코드 기반 문자열 포인터
                     _In_ int       nCmdShow)			// 실행한 창의 스타일은 지정하는 flag 값 (모양 / 최소화, 최대화 모양 등등의 옵션)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    MSG msg;
	msg.message = WM_NULL;

	CMainGame		MainGame;
	MainGame.Initialize();

	DWORD dwTime = GetTickCount();		// 955

	while (true)
	{
		// PM_REMOVE		: 메세지를 읽어옴과 동시에 메세지 큐에서 제거
		// PM_NOREMOVE		: 메세지 큐에 메세지가 존재하는지만 파악, 만약 메세지를 읽어오려면 GetMessage를 다시 호출해야 함
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
			    TranslateMessage(&msg);
			    DispatchMessage(&msg);
			}
		}

		else
		{	
			// 1050 + 10		< 959
			if (dwTime + 10 < GetTickCount())
			{			
				MainGame.Update();
				MainGame.Late_Update();
				MainGame.Render();

				dwTime = GetTickCount();		// 1050
			}	

			/*MainGame.Update();
			MainGame.Late_Update();
			MainGame.Render();*/
		}
	}



    return (int) msg.wParam;
}	 // 



//
//  함수: MyRegisterClass()
//
//  목적: 현재 윈도우를 만들기 위해 윈도우에 대한 설정을 기록
//	어떤 설정에 어떤 속성을 가질지를 설정하는 함수

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//WNDCLASSEXW : 윈도우를 만들 때 창의 각종 정보를 저장하는 용도의 구조체

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);		// 자기 자신의 사이즈를 저장해 놓음

    wcex.style          = CS_HREDRAW | CS_VREDRAW;	// 윈도우 창의 스타일을 정의, 초기화 되는 값이 의미하는 것은 윈도우 창의 수작, 또는 수평의 크기가 변할 경우 윈도우를 다시 그린다는 의미
		
	// CS_HREDRAW : 가로를 다시 그린다.
	// CS_VREDRAW : 세로를 다시 그린다.

    wcex.lpfnWndProc    = WndProc;	// 메세지 처리하는 함수, 저장해둔 함수가 호출되어 메세지 처리를 수행

    wcex.cbClsExtra     = 0;	// 윈도우가 특수한 목적으로 사용하는 여분의 공간    
	wcex.cbWndExtra     = 0;	// 일종의 예약 영역으로 0을 지정하면 예약 영역을 사용하지 않겠다는 뜻

    wcex.hInstance      = hInstance;	//메인 함수에서 매개 변수로 전달된 값, 프로그램의 번호를 설정

    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
						// 윈도우 창이 사용할 아이콘을 지정(시작 표시줄)

    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
						// 윈도우 창에서 사용할 마우스 커서를 지정

    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
						// 윈도우 작업 영역의 배경 색을 설정

	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
						// 창 상단에 있는 메뉴 설정

    wcex.lpszClassName  = szWindowClass;
						// 실행 파일의 이름을 저장(가급적이면 프로젝트 이름과 일치시킴, 건드리지마)

    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
						// 창 상단에 출력할 작은 아이콘 모양 지정

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // CreateWindowW : 등록해 놓은 정보들을 토대로 창을 만드는 함수

   RECT		rcWnd{ 0, 0, WINCX, WINCY };

  //  rcWnd = rcWnd + 기본 창 스타일 + 메뉴 바 사이즈 고려
   AdjustWindowRect(&rcWnd, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass,			// 앞서 정의한 윈도우의 전달할 이름
							   szTitle,				// 창 상단에 출력할 문자열
							   WS_OVERLAPPEDWINDOW,	// 창 스타일 지정 옵션(기본 값으로 사용하는 모양)
							   CW_USEDEFAULT, 0,	// 창을 출력할 X, Y 좌표
							   rcWnd.right - rcWnd.left, 
							   rcWnd.bottom - rcWnd.top,			// 창의 가로, 세로 사이즈
							   nullptr,				// 부모 윈도우가 있다면 부모 윈도우의 핸들을 지정	
							   nullptr,				// 윈도우에서 사용할 메뉴의 핸들을 지정, 기본값 쓰려면 NULL
							   hInstance,			// 윈도우를 만드는 주체
							   nullptr);			// 운영 체제가 특수한 목적으로 사용

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 기본적인 라이브러리
// 1. KERNEL : 메모리를 관리하고 프로그램을 실행
// 2. USER : 유저 인터페이스와 윈도우(창)를 관리
// 3. GDI : 화면 처리와 그래픽을 담당

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;	

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			//PostQuitMessage(0);
			DestroyWindow(hWnd);
			break;
		}

		break;


    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// 함수 호출 규약

// : 함수의 실행이 종료되면 할당 되었던 스택(스택 프레임)을 해제해야 하는데 함수를 호출한 호출원(caller)에서 정리할 것인지
// 아니면 호출받은 피호출자(callee)에서 할 것인가에 대한 약속

// __cdecl, __stdcall, __fastcall, __thiscall

// __cdecl    : c/c++ 언어의 기본 호출 규약, 호출원이 stack을 정리, 어떤 매개 변수가 얼마나 들어올지 모르는 문제(가변 인자 함수) 때문에 함수를 호출한 호출원이 메모리를 계산하여 반환
// __stdcall  : win32 api 기본 호출 규약, 피호출자가 stack을 정리, api에서 제공하는 대부분의 함수들은 고정 인자 함수, 매개 변수가 고정되어 있어 피호출자가 어떤 것이 메모리 상에 등록될지 알고 있어서 스스로 stack을 정리할 수 있음
// __fastcall : 함수 호출을 빠르게 처리하기 위한 호출 규약, 레지스터를 사용하여 함수 호출이 빨라짐
// __thiscall : this 포인터를 매개 변수로 넘겨 받았을 때(멤버 함수에만 적용)