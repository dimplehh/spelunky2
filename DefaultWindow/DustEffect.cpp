#include "stdafx.h"
#include "DustEffect.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CDustEffect::CDustEffect()
{
}

CDustEffect::~CDustEffect()
{
	Release();
}

void CDustEffect::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
		
	m_iSize = 16;

	m_pFrameKey = L"DustEffect";

	m_eEffectID = EFFECT_DUST;

	m_eRender = RENDER_EFFECT;
}

int CDustEffect::Update()
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CDustEffect::Late_Update()
{
}

void CDustEffect::Render(HDC hDC)
{
	if (m_bIsActive == false)
		return;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(6, 38, 76));
}

void CDustEffect::Release()
{
}
