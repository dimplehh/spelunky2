#include "stdafx.h"
#include "Frog.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Player.h"

CFrog::CFrog() :m_ePreState(ST_END), m_eCurState(JUMP), m_dwTime(GetTickCount())
{
}

CFrog::~CFrog()
{
}

void CFrog::Initialize()
{
	m_tInfo = { 0.f, 0.f, TILECX, TILECY };
	m_fSpeed = 1.5f;
	m_iHp = 1;
	m_iAttackPower = 1;
	m_pFrameKey = L"Frog";
	m_tFrame = { 0, 0, 0, 5, true, 60, GetTickCount(), 0 };
	m_eRender = RENDER_GAMEOBJECT;
}

int CFrog::Update()
{
	if (m_iHp <= 0)
		return OBJ_DEAD;

	Idle();
	Attack();
	Damaged();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CFrog::Late_Update()
{
	Gravity();
	Motion_Change();
	__super::Move_Frame();
}

void CFrog::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(33, 33, 33));
}

void CFrog::Release()
{
}

void CFrog::Motion_Change()
{
	m_bCurFlip = m_bFlip;

	if (m_ePreState != m_eCurState || m_bPreFlip != m_bCurFlip)
	{
		m_iRepeatCount = 0;
		switch (m_eCurState)
		{
		case CFrog::IDLE:		Set_Frame(0, 0, 0, true, 120, 0, 5);		break;
		case CFrog::JUMP:		Set_Frame(0, 5, 0, true, 120, 0, 5);		break;
		}
		m_ePreState = m_eCurState;
		m_bPreFlip = m_bCurFlip;
	}
}

void CFrog::Idle()
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
			m_pFrameKey = L"FrogFlip";
			m_tInfo.fX -= 10;
		}
		else
		{
			m_bFlip = false;
			m_pFrameKey = L"Frog";
			m_tInfo.fX += 10;
		}
	}
}

void CFrog::Attack()
{
	if (m_eCurState == JUMP)
	{
		if (m_bFirstAttack == true)
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHp(-m_iAttackPower);
			m_bFirstAttack = false;
		}
	}
	else
		m_bFirstAttack = true;
}

void CFrog::Damaged()
{
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetState() == CPlayer::ATTACK)
	{
		INFO _playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

		if (((m_tInfo.fX - m_tInfo.fCX <= _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX - m_tInfo.fCX / 2)
			|| (m_tInfo.fX + m_tInfo.fCX / 2 < _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX + m_tInfo.fCX))
			&& m_tInfo.fY - m_tInfo.fCY / 2 <= _playerInfo.fY && _playerInfo.fY <= m_tInfo.fY + m_tInfo.fCY / 2)
		{
			if (m_bFirstDamaged == true)
			{
				m_iHp -= 1;
				m_bFirstDamaged = false;
			}
		}
	}
	else
		m_bFirstDamaged = true;

}