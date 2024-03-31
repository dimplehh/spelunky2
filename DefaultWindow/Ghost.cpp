#include "stdafx.h"
#include "Ghost.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Player.h"

CGhost::CGhost() :m_ePreState(ST_END), m_eCurState(IDLE), m_dwTime(GetTickCount())
{
}

CGhost::~CGhost()
{
}

void CGhost::Initialize()
{
	m_tInfo = { 0.f, 0.f, 128.f, 128.f };
	m_fSpeed = 1.5f;
	m_iHp = 1;
	m_iAttackPower = 1;
	m_pFrameKey = L"Ghost";
	m_tFrame = { 0, 5, 0, 5, true, 60, GetTickCount(), 0 };
	m_eRender = RENDER_GHOST;
}

int CGhost::Update()
{
	if (m_iHp <= 0)
		return OBJ_DEAD;

	Idle();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGhost::Late_Update()
{
	Gravity();
	Motion_Change();
	__super::Move_Frame();


	//#ifdef _DEBUG
	//
	//	if (m_dwTime + 1000 < GetTickCount())
	//	{
	//		cout << m_iHp << endl;
	//		m_dwTime = GetTickCount();
	//	}
	//#endif
}

void CGhost::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left , m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(61, 61, 61));
}

void CGhost::Release()
{
}

void CGhost::Motion_Change()
{
	m_bCurFlip = m_bFlip;

	if (m_ePreState != m_eCurState || m_bPreFlip != m_bCurFlip)
	{
		m_iRepeatCount = 0;
		switch (m_eCurState)
		{
		case CGhost::IDLE:			Set_Frame(0, 5, 0, true, 120, 0, 5);		break;
		case CGhost::ATTACK:		Set_Frame(0, 5, 1, true, 120, 0, 5);		break;
		}
		m_ePreState = m_eCurState;
		m_bPreFlip = m_bCurFlip;
	}
}

void CGhost::Idle()
{
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
			m_pFrameKey = L"GhostFlip";
			m_tInfo.fX -= 10;
		}
		else
		{
			m_bFlip = false;
			m_pFrameKey = L"Ghost";
			m_tInfo.fX += 10;
		}
	}
}
