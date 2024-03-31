#include "stdafx.h"
#include "Snake.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"

CSnake::CSnake()  :m_ePreState(ST_END), m_eCurState(IDLE), m_dwTime(GetTickCount())
{
}

CSnake::~CSnake()
{
}

void CSnake::Initialize()
{
	m_tInfo = { 0.f, 0.f, TILECX, TILECY };
	m_fSpeed = 1.5f;
	m_iHp = 1;
	m_iAttackPower = 1;
	m_pFrameKey = L"Snake"; 
	m_tFrame = { 0, 10, 0, 10, true, 60, GetTickCount(), 0 };
	m_eRender = RENDER_GAMEOBJECT;
}

int CSnake::Update()
{
	if (m_iHp < 0)
		return OBJ_DEAD;

	if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
	{
		m_tInfo.fX += m_fSpeed;
	}
	else
	{
		m_fSpeed = -m_fSpeed;
		if (m_fSpeed < 0)
		{
			m_bFlip = true; 
			m_pFrameKey = L"SnakeFlip";
			m_tInfo.fX -= 10;
		}
		else
		{
			m_bFlip = false;
			m_pFrameKey = L"Snake";
			m_tInfo.fX += 10;
		}
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSnake::Late_Update()
{
	Gravity();
	Motion_Change();
	__super::Move_Frame();
}

void CSnake::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 
					m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(53, 53,53));

#ifdef _DEBUG

	if (m_dwTime + 1000 < GetTickCount())
	{
		cout << m_bFlip << endl;
		m_dwTime = GetTickCount();
	}
#endif
}

void CSnake::Release()
{
}

void CSnake::Motion_Change()
{
	m_bCurFlip = m_bFlip;

	if (m_ePreState != m_eCurState || m_bPreFlip != m_bCurFlip)
	{
		m_iRepeatCount = 0;
		switch (m_eCurState)
		{
		case CSnake::IDLE:			Set_Frame(0, 10, 0, true, 120, 0, 10);		break;
		case CSnake::ATTACK:		Set_Frame(0, 6, 1, true, 120, 0, 10);		break;
		}
		m_ePreState = m_eCurState;
		m_bPreFlip = m_bCurFlip;
	}
}
