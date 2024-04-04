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

	m_iSize = m_iMinSize;

	m_pFrameKey = L"StarEffect";

	m_eEffectID = EFFECT_STAR;
	m_tFrame = { 0, 3, 0, 3, true, 200, GetTickCount(), 1 };
	Set_Frame(0, 3, 0, true, 200, 0, 1);

	m_eRender = RENDER_EFFECT;
}

int CStarEffect::Update() //크기도 점점 커지게
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	if (m_bCheckFirstTime == true)
	{
		int xs[4] = { 2, 1, -1, -2};
		int ys[4] = { -2, 2, -2, 1};

		int random = rand() % 4;
		m_fAddX = xs[random];
		m_fAddY = ys[random];

		m_dwTime = GetTickCount();
		m_bCheckFirstTime = false;
	}

	m_tInfo.fX += m_fAddX;
	m_tInfo.fY += m_fAddY;
	if (m_iSize < m_iMaxSize)
		m_iSize += 1;

	if (m_dwTime + 300 < GetTickCount())
	{
		m_bIsActive = false;
		m_bCheckFirstTime = true;
		m_iSize = m_iMinSize;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CStarEffect::Late_Update()
{
	__super::Move_Frame();
}

void CStarEffect::Render(HDC hDC)
{
	if (m_bIsActive == false)
		return;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, (m_tInfo.fX - m_tInfo.fCX / 2) + iScrollX, (m_tInfo.fY - m_tInfo.fCY / 2) + iScrollY, (int)m_iSize, (int)m_iSize, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0 * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(45, 45, 45));
}

void CStarEffect::Release()
{
}
