#include "stdafx.h"
#include "Stage4.h"
#include "Player.h"
#include "ObjMgr.h"
#include "EffectMgr.h"
#include "TileMgr.h"
#include "LineMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CStage4::CStage4()
{
}

CStage4::~CStage4()
{
}

void CStage4::Initialize()
{
	__super::m_iMapNum = 4;
	float _responPosX = TILECX * (4 + 0.5f);
	float _responPosY = TILECY * (5.5f);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetRespawnPos(_responPosX, _responPosY);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(_responPosX, _responPosY);

	InsertBmps();
	InsertGimics();

	CEffectMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Load_Tile(4);
	CLineMgr::Get_Instance()->Initialize();

	CScrollMgr::Get_Instance()->Set_ScrollXY(WINCX / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX,
		WINCY / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);
	CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_MAP, __super::m_iMapNum);

	_bf.BlendOp = AC_SRC_OVER;
	_bf.BlendFlags = 0;
	_bf.AlphaFormat = AC_SRC_ALPHA;
	_bf.SourceConstantAlpha = 0;
}

int CStage4::Update()
{
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	return 0;
}

void CStage4::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
}

void CStage4::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground4");
	// 비트맵 출력 함수
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, iScrollX, iScrollY, TILECX * TILEX, TILECY * TILEY, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
	CTileMgr::Get_Instance()->Render(hDC);
	CLineMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->RenderUI(hDC);		// ui 가장 마지막에 렌더
	FadeInOut(hDC);								// fade 효과 그보다 더 마지막에 렌더
}

void CStage4::Release()
{
	CObjMgr::Get_Instance()->ReleaseWithoutPlayerAndUI();
}

void CStage4::InsertBmps()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Ground4.bmp", L"Ground4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Fade.bmp", L"Fade");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/FadeIn.bmp", L"FadeIn");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette7.bmp", L"Tile7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Palette8.bmp", L"Tile8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/UIIcon.bmp", L"UIIcon");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Box.bmp", L"Box");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Item.bmp", L"Item");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/Chest.bmp", L"Chest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Object/HoldObj.bmp", L"HoldObj");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/DecoLand4.bmp", L"DecoLand4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Font/Font.bmp", L"Font");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/DustEffect.bmp", L"DustEffect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BlinkEffect.bmp", L"BlinkEffect");
}

void CStage4::InsertChests()
{
}

void CStage4::InsertHoldObjs()
{
}

void CStage4::InsertEffects()
{
}

void CStage4::InsertGimics()
{
	InsertChests();
	InsertHoldObjs();
	InsertEffects();
}

void CStage4::ReleaseGimics()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_CHEST);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_HOLDOBJ);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_EFFECT);
}

void CStage4::FadeInOut(HDC hDC)
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
	else if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetRevival() == true
		|| dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetCheckFirstInit() == true)			// FADEIN
	{
		m_fAlpha = 0.f;
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FadeIn");

		GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, (m_fFadeIn / 2) * WINCX, 0, WINCX, WINCY, RGB(55, 55, 55));

		m_fFadeIn++;

		if (m_fFadeIn / 2 > 9)
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetRevival(false);
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetCheckFirstInit(false);
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetResetFirstTime(true);
			m_fFadeIn = 0;
		}
	}
}
