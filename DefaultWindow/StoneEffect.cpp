#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "StoneEffect.h"

CStoneEffect::CStoneEffect():m_fTime(0.f)
{
}

CStoneEffect::~CStoneEffect()
{
	Release();
}

void CStoneEffect::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_iSize = 32;

	m_pFrameKey = L"StoneEffect";

	m_eEffectID = EFFECT_STONE;
	m_tFrame = { 0, 3, 0, 3, true, 300, GetTickCount(), 1 };
	Set_Frame(0, 3, 0, true, 300, 0, 4);

	m_eRender = RENDER_EFFECT;
}

int CStoneEffect::Update()
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	if (m_bCheckFirstTime == true)
	{
		int xs[13] = { -18,-15,-12, -9, -5, -2, 2 ,5, 8, 10, 12 ,15, 18};
		m_fAddX = xs[rand() % 13];
		m_fPreY = m_tInfo.fY;
		m_dwTime = GetTickCount();
		m_bCheckFirstTime = false;
	}

	m_tInfo.fX += m_fAddX;
	m_tInfo.fY = m_fPreY - 35.f * m_fTime + ((9.8f * m_fTime * m_fTime) * 0.5f);
	m_fTime += 0.5f;

	if (m_dwTime + 500 < GetTickCount())
	{
		m_bIsActive = false;
		m_bCheckFirstTime = true;
		m_fTime = 0.f;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CStoneEffect::Late_Update()
{
	__super::Move_Frame();
}

void CStoneEffect::Render(HDC hDC)
{
	if (m_bIsActive == false)
		return;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, (m_tInfo.fX - m_tInfo.fCX / 2) + iScrollX, (m_tInfo.fY - m_tInfo.fCY / 2) + iScrollY, (int)m_iSize, (int)m_iSize, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0 * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(45, 45, 45));
}

void CStoneEffect::Release()
{
}
