#include "stdafx.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "Monster.h"
#include "LineMgr.h"
#include "Box.h"
#include "UIIcon.h"

CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette2.bmp", L"Tile2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/UIIcon.bmp", L"UIIcon");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Box.bmp", L"Box");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Rope.bmp", L"Rope");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monster.bmp", L"Monster");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/DecoLand.bmp", L"DecoLand");

	CObj* pObj = CAbstractFactory<CUIIcon>::Create(50.f, 50.f);
	dynamic_cast<CUIIcon*>(pObj)->Set_FrameKey(L"UIIcon");
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);

	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());

	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (59 + 0.5f), TILECY * 10));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (38 + 0.5f), TILECY * 17));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (20 + 0.5f), TILECY * 6));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (15 + 0.5f), TILECY * 1));

	CTileMgr::Get_Instance()->Load_Tile();
	CLineMgr::Get_Instance()->Initialize();
	CScrollMgr::Get_Instance()->Set_ScrollXY(WINCX / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX, WINCY - -CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);
}

int CStage::Update()
{
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	return 0;
}

void CStage::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");
	// 비트맵 출력 함수
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, TILECX * TILEX, TILECY * TILEY, hGroundDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(hDC);
	CLineMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_UI);
}
