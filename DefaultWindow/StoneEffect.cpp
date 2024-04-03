#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "StoneEffect.h"

CStoneEffect::CStoneEffect()
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

	m_iSize = 16;

	m_pFrameKey = L"StoneEffect";

	m_eEffectID = EFFECT_STONE;

	m_eRender = RENDER_EFFECT;
}

int CStoneEffect::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CStoneEffect::Late_Update()
{
}

void CStoneEffect::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(53, 53, 53));
}

void CStoneEffect::Release()
{
}
