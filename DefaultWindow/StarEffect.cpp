#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "StarEffect.h"

CStarEffect::CStarEffect()
{
}

CStarEffect::~CStarEffect()
{
	Release();
}

void CStarEffect::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_iSize = 16;

	m_pFrameKey = L"StarEffect";

	m_eEffectID = EFFECT_STAR;

	m_eRender = RENDER_EFFECT;
}

int CStarEffect::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CStarEffect::Late_Update()
{
}

void CStarEffect::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(53, 53, 53));
}

void CStarEffect::Release()
{
}
