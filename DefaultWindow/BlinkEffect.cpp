#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "BlinkEffect.h"

CBlinkEffect::CBlinkEffect() : m_dwTime(GetTickCount())
{
}

CBlinkEffect::~CBlinkEffect()
{
	Release();
}

void CBlinkEffect::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_iSize = m_iMinSize;

	m_pFrameKey = L"BlinkEffect";

	m_eEffectID = EFFECT_BLINK;
	m_tFrame = { 0, 1, 0, 1, true, 500, GetTickCount(), 1 };
	Set_Frame(0, 1, 0, true, 500, 0, 1);

	m_eRender = RENDER_EFFECT;
}

int CBlinkEffect::Update()
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	if (m_bCheckFirstTime == true)
	{
		int xs[4] = { 15, 10, -5, -15 };
		int ys[4] = { -5, 10, -15, 5 };

		int random = rand() % 4;
		int ranSize = m_iMinSize + rand() % (m_iMaxSize - m_iMinSize);
		
		m_tInfo.fX += xs[random];
		m_tInfo.fY += ys[random];
		m_iSize = ranSize;

		m_dwTime = GetTickCount();
		m_bCheckFirstTime = false;
	}

	if (m_iSize < m_iMaxSize)
		m_iSize++;
	else
		m_iSize--;

	if (m_dwTime + 300 < GetTickCount())
	{
		m_bIsActive = false;
		m_bCheckFirstTime = true;
		m_iSize = m_iMinSize;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBlinkEffect::Late_Update()
{
	__super::Move_Frame();
}

void CBlinkEffect::Render(HDC hDC)
{
	if (m_bIsActive == false)
		return;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, (m_tInfo.fX - m_tInfo.fCX / 2) + iScrollX, (m_tInfo.fY - m_tInfo.fCY / 2) + iScrollY, (int)m_iSize, (int)m_iSize, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0 * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(45, 45, 45));
}

void CBlinkEffect::Release()
{
}
