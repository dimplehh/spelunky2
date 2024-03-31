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
#include "HoldObj.h"
#include "Obstacle.h"
#include "UIIcon.h"
#include "UIMgr.h"
#pragma comment(lib, "msimg32.lib")

CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	__super::m_iMapNum = 1;

	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	InsertBmps();
	InsertMonsters();
	InsertUIs();
	InsertObstacles();
	InsertBoxs();
	InsertChests();
	InsertHoldObjs();
	CTileMgr::Get_Instance()->Load_Tile();
	CLineMgr::Get_Instance()->Initialize();
	CScrollMgr::Get_Instance()->Set_ScrollXY(	WINCX / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX, 
												WINCY - -CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);
	_bf.BlendOp = AC_SRC_OVER;
	_bf.BlendFlags = 0;
	_bf.AlphaFormat = AC_SRC_ALPHA;
	_bf.SourceConstantAlpha = 0;
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
	CUIMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");
	// 비트맵 출력 함수
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, TILECX * TILEX, TILECY * TILEY, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
	CTileMgr::Get_Instance()->Render(hDC);
	CLineMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->RenderGhost(hDC);	// ui 전에 유령 렌더
	CObjMgr::Get_Instance()->RenderUI(hDC);		// ui 가장 마지막에 렌더
	FadeInOut(hDC);								// fade 효과 그보다 더 마지막에 렌더
}

void CStage::FadeInOut(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Fade");
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetHp() == 0)								//FADEOUT
	{
		m_fAlpha += 2.f;
		if (m_fAlpha >= 255.f)
			m_fAlpha = 255.f;

		_bf.SourceConstantAlpha = m_fAlpha;

		AlphaBlend(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, WINCX, WINCY, _bf);
	}
	else if (	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetRevival() == true
			||	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetCheckFirstInit() == true)			// FADEIN
	{
		m_fAlpha = 0.f;
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FadeIn");

		GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, (m_fFadeIn / 2) * WINCX, 0, WINCX, WINCY, RGB(55, 55, 55));

		m_fFadeIn++;

		if (m_fFadeIn / 2 > 9)
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetRevival(false);
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetCheckFirstInit(false);
			m_fFadeIn = 0;
		}
	}
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_UI);
}

void CStage::InsertBmps()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground.bmp",	L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Fade.bmp",	L"Fade");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/FadeIn.bmp",	L"FadeIn");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette.bmp",		L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette2.bmp",		L"Tile2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/UIIcon.bmp",		L"UIIcon");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Box.bmp",			L"Box");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Rope.bmp",		L"Rope");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Bomb2.bmp",		L"Bomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Item.bmp",		L"Item");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Obstacle.bmp",	L"Obstacle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Chest.bmp",		L"Chest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/HoldObj.bmp",		L"HoldObj");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Snake.bmp",		L"Snake");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SnakeFlip.bmp",	L"SnakeFlip");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Ghost.bmp",		L"Ghost");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/GhostFlip.bmp",	L"GhostFlip");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/GhostUp.bmp",	L"GhostUp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/DecoLand.bmp",		L"DecoLand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Font/Font.bmp",			L"Font");
}

void CStage::InsertUIs()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CUIFactory::Create(50.f, 50.f, CUIIcon::UI_HP, dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetHp(), 18));
	CUIMgr::Get_Instance()->Insert_UI(CUIIcon::UI_HP, dynamic_cast<CUIIcon*>(CObjMgr::Get_Instance()->Get_UI()));
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CUIFactory::Create(50.f + TILECX, 50.f, CUIIcon::UI_ROPE, dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetRopeCount(), 16, 0.f, 2.f));
	CUIMgr::Get_Instance()->Insert_UI(CUIIcon::UI_ROPE, dynamic_cast<CUIIcon*>(CObjMgr::Get_Instance()->Get_UI()));
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CUIFactory::Create(50.f + TILECX * 2, 50.f, CUIIcon::UI_BOMB, dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetBombCount(), 16, 0.f, 2.f));
	CUIMgr::Get_Instance()->Insert_UI(CUIIcon::UI_BOMB, dynamic_cast<CUIIcon*>(CObjMgr::Get_Instance()->Get_UI()));
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CUIFactory::Create(WINCX - TILECX * 6, 50.f, CUIIcon::UI_MONEY, dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetMoney(), 15, 30.f, -7.f));
	CUIMgr::Get_Instance()->Insert_UI(CUIIcon::UI_MONEY, dynamic_cast<CUIIcon*>(CObjMgr::Get_Instance()->Get_UI()));
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CUIFactory::Create(WINCX - TILECX * 4, 50.f, CUIIcon::UI_TIME, 0, 15, 10.f, -7.f));
	CUIMgr::Get_Instance()->Insert_UI(CUIIcon::UI_TIME, dynamic_cast<CUIIcon*>(CObjMgr::Get_Instance()->Get_UI()));
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CUIFactory::Create(WINCX - TILECX * 2, 50.f, CUIIcon::UI_MAP, __super::m_iMapNum, 15, 10.f, -7.f));
	CUIMgr::Get_Instance()->Insert_UI(CUIIcon::UI_MAP, dynamic_cast<CUIIcon*>(CObjMgr::Get_Instance()->Get_UI()));
}

void CStage::InsertMonsters()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CMonsterFactory::Create(TILECX * (21 + 0.5f), TILECY * (6.5f), CMonster::SNAKE));
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CMonsterFactory::Create(WINCX + 50.f, -50.f , CMonster::GHOST));
}

void CStage::InsertObstacles()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_OBSTACLE, CAbstractFactory<CObstacle>::Create(TILECX * (27 + 0.5f), TILECY * (6 + 0.5f)));
}

void CStage::InsertBoxs()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (59 + 0.5f), TILECY * 10));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (38 + 0.5f), TILECY * 17));
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOX, CBoxFactory::Create(TILECX * (15 + 0.5f), TILECY * 1));
}

void CStage::InsertChests()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_CHEST, CChestFactory::Create(TILECX * (20 + 0.5f), TILECY * (6.5f), CItem::ITEM_BOMB));
	CObjMgr::Get_Instance()->Add_Object(OBJ_CHEST, CChestFactory::Create(TILECX * (22 + 0.5f), TILECY * (6.5f), CItem::ITEM_ROPE));
	CObjMgr::Get_Instance()->Add_Object(OBJ_CHEST, CChestFactory::Create(TILECX * (24 + 0.5f), TILECY * (6.5f), CItem::ITEM_GEM));
	CObjMgr::Get_Instance()->Add_Object(OBJ_CHEST, CChestFactory::Create(TILECX * (26 + 0.5f), TILECY * (6.5f), CItem::ITEM_GOLD));
}

void CStage::InsertHoldObjs()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_HOLDOBJ, CHoldObjFactory::Create(TILECX * (16 + 0.5f), TILECY * (4 + 0.5f), CHoldObj::HOLDOBJ_JAR));
	CObjMgr::Get_Instance()->Add_Object(OBJ_HOLDOBJ, CHoldObjFactory::Create(TILECX * (18 + 0.5f), TILECY * (4 + 0.5f), CHoldObj::HOLDOBJ_KEY));
	CObjMgr::Get_Instance()->Add_Object(OBJ_HOLDOBJ, CHoldObjFactory::Create(TILECX * (19 + 0.5f), TILECY * (6 + 0.5f), CHoldObj::HOLDOBJ_STONE));
}
