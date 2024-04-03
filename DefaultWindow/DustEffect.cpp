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
		
	m_iSize = 26;

	m_pFrameKey = L"DustEffect";

	m_eEffectID = EFFECT_DUST;
	m_tFrame = { 0, 0, 0, 2, true, 200, GetTickCount(), 1 };
	Set_Frame(0, 2, 0, true, 200, 0, 1);

	m_eRender = RENDER_EFFECT;
}

int CDustEffect::Update()
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	if (m_bCheckFirstTime == true)
	{
		int xs[5] = { 0, 5, 10, 20, 25 };

		m_tInfo.fX += xs[rand() % 5];

		m_dwTime = GetTickCount();
		m_bCheckFirstTime = false;
	}
	
	if (m_dwTime + 500 < GetTickCount())
	{
		m_bIsActive = false;
		m_bCheckFirstTime = true;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CDustEffect::Late_Update()
{
	__super::Move_Frame();
}

void CDustEffect::Render(HDC hDC)
{
	if (m_bIsActive == false)
		return;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, (m_tInfo.fX - m_tInfo.fCX / 2) + iScrollX, (m_tInfo.fY - m_tInfo.fCY / 2) + iScrollY, (int)m_iSize, (int)m_iSize, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0 * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(45, 45, 45));
}

void CDustEffect::Release()
{
}
