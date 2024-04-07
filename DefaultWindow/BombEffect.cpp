#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "BombEffect.h"

CBombEffect::CBombEffect()
{
}

CBombEffect::~CBombEffect()
{
	Release();
}

void CBombEffect::Initialize()
{
	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_pFrameKey = L"BombEffectBase";

	m_eEffectID = EFFECT_BOMB;
	m_tFrame = { 0, 24, 0, 24, false, 10, GetTickCount(), 1 };
	Set_Frame(0, 24, 0, false, 10, 1, 24);
	m_iSize = m_iMinSize;
	m_eRender = RENDER_EFFECT;
}

int CBombEffect::Update()
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	if (m_bCheckFirstTime == true)
	{
		m_dwTime = GetTickCount();
		m_bCheckFirstTime = false;
	}

	m_iSize += 1;

	if (m_dwTime + 1000 < GetTickCount())
	{
		m_bIsActive = false;
		m_bCheckFirstTime = true;
		m_iSize = m_iMinSize;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBombEffect::Late_Update()
{
	__super::Move_Frame();
}

void CBombEffect::Render(HDC hDC)
{
	if (m_bIsActive == false)
		return;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, (m_tInfo.fX - m_tInfo.fCX) + iScrollX, (m_tInfo.fY - m_tInfo.fCY) + iScrollY, (int)m_iSize, (int)m_iSize, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0 * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(45, 45, 45));
}

void CBombEffect::Release()
{
}
