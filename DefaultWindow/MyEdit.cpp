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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile2.bmp", L"Tile");
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
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");
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

		CTileMgr::Get_Instance()->Picking_Tile(pt, 1, 1);
	}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::D) == KEY_STATE::TAP)
	{
		CTileMgr::Get_Instance()->Picking_Tile(pt, 0, 0);
	}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::F1) == KEY_STATE::TAP)
		CTileMgr::Get_Instance()->Save_Tile();

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::F2) == KEY_STATE::TAP)
		CTileMgr::Get_Instance()->Load_Tile();
}
