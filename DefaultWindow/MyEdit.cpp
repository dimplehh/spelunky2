#include "stdafx.h"
#include "MyEdit.h"
#include "TileMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CMyEdit::CMyEdit()
{
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground4.bmp", L"Ground4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette7.bmp", L"Tile7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette8.bmp", L"Tile8");
	CTileMgr::Get_Instance()->Initialize();
}

int CMyEdit::Update()
{
	Key_Input();

	CTileMgr::Get_Instance()->Update();

	return 0;
}

void CMyEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CMyEdit::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground4");
	// 비트맵 출력 함수
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, TILECX * TILEX, TILECY * TILEY, hGroundDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(hDC);
}

void CMyEdit::Release()
{
}

void CMyEdit::Key_Input()
{
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
		CScrollMgr::Get_Instance()->Set_ScrollX(12.f);

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
		CScrollMgr::Get_Instance()->Set_ScrollX(-12.f);

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
		CScrollMgr::Get_Instance()->Set_ScrollY(12.f);

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
		CScrollMgr::Get_Instance()->Set_ScrollY(-12.f);

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
	{
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Instance()->Get_ScrollY();

		CTileMgr::Get_Instance()->Picking_Tile(pt, m_iIndex, m_iOption, m_pFrameKey);
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RBUTTON) == KEY_STATE::TAP)
	{
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Instance()->Get_ScrollY();

		CTileMgr::Get_Instance()->Picking_Tile(pt, 0, 0, m_pFrameKey);
	}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::Q) == KEY_STATE::TAP)
	{
		--m_iIndex;
		m_iOption = 1;
	}

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::E) == KEY_STATE::TAP)
	{
		++m_iIndex;
		m_iOption = 1;
	}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::A) == KEY_STATE::TAP)
	{
		--m_iIndex;
		m_iOption = 2;
	}

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::D) == KEY_STATE::TAP)
	{
		++m_iIndex;
		m_iOption = 2;
	}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::F1) == KEY_STATE::TAP)
		CTileMgr::Get_Instance()->Save_Tile();

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::F2) == KEY_STATE::TAP)
		CTileMgr::Get_Instance()->Load_Tile();
}
