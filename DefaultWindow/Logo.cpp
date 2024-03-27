#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");
	m_hVideo = MCIWndCreate(g_hWnd,
							nullptr,
							WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR,
							L"../Video/Title.wmv");
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	MCIWndPlay(m_hVideo);
}

int CLogo::Update()
{
	return 0;
}

void CLogo::Late_Update()
{
	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
		MCIWndPlay(m_hVideo);
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
		return;
	}
}


void CLogo::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
	MCIWndClose(m_hVideo);
}
