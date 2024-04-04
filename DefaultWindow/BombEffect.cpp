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

	m_pFrameKey = L"BombEffect";

	m_eEffectID = EFFECT_BOMB;
	m_tFrame = { 0, 3, 0, 3, true, 200, GetTickCount(), 1 };
	Set_Frame(0, 3, 0, true, 200, 0, 1);

	m_eRender = RENDER_EFFECT;
}

int CBombEffect::Update()
{
	if (m_bIsActive == false)
		return OBJ_NOEVENT;

	if (m_bCheckFirstTime == true)
	{
		float xs[8] = { 1, -1, 1, -1, 0, 0, 1.4f, -1.4f};
		float ys[8] = {1, 1, -1, -1, 1.4f, -1.4f, 0, 0};

		int random = rand() % 8;
		m_fAddX = xs[random] * 0.2f;
		m_fAddY = ys[random] * 0.2f;

		int random2 = rand() % 8;
		m_iSpeed = random2 * 2;

		m_dwTime = GetTickCount();
		m_bCheckFirstTime = false;
	}

	m_tInfo.fX += m_fAddX * m_iSpeed;
	m_tInfo.fY += m_fAddY * m_iSpeed;;

	if (m_dwTime + 500 < GetTickCount())
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

	GdiTransparentBlt(hDC, (m_tInfo.fX - m_tInfo.fCX / 2) + iScrollX, (m_tInfo.fY - m_tInfo.fCY / 2) + iScrollY, (int)m_iSize, (int)m_iSize, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0 * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(45, 45, 45));
}

void CBombEffect::Release()
{
}
