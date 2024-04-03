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
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_iSize = 16;

	m_pFrameKey = L"BombEffect";

	m_eEffectID = EFFECT_BOMB;

	m_eRender = RENDER_EFFECT;
}

int CBombEffect::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBombEffect::Late_Update()
{
}

void CBombEffect::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(53, 53, 53));
}

void CBombEffect::Release()
{
}
