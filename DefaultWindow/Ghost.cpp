#include "stdafx.h"
#include "Ghost.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Player.h"
#include "UIMgr.h"
#pragma comment(lib, "msimg32.lib")

CGhost::CGhost() :m_ePreState(ST_END), m_eCurState(IDLE), m_dwTime(GetTickCount())
{
}

CGhost::~CGhost()
{
}

void CGhost::Initialize()
{
	m_tInfo = { m_fFirstX, m_fFirstY, 128.f, 128.f };
	m_fSpeed = 0.6f;
	m_iHp = 1;
	m_iAttackPower = 1;
	m_pFrameKey = L"Ghost";
	m_tFrame = { 0, 5, 0, 5, true, 60, GetTickCount(), 0 };
	m_eRender = RENDER_GHOST;
	__super::Update_Rect();
}

int CGhost::Update()
{
	if (m_bDead == true)
		return OBJ_DEAD;

	if (CUIMgr::Get_Instance()->Get_Time() == 0)
	{
		m_tInfo.fX = m_fFirstX;
		m_tInfo.fY = m_fFirstY;
		__super::Update_Rect();
	}

	if (CUIMgr::Get_Instance()->Get_Time() < 10) //일단 10초 지나면 등장함
		return OBJ_NOEVENT;

	Follow();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGhost::Late_Update()
{
	Motion_Change();
	__super::Move_Frame();
}

void CGhost::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	BLENDFUNCTION	_bf = {};
	_bf.BlendOp = AC_SRC_OVER;
	_bf.BlendFlags = 0;
	_bf.AlphaFormat = AC_SRC_ALPHA;
	_bf.SourceConstantAlpha = 0;
	_bf.SourceConstantAlpha = 180.f;

	if (m_pFrameKey == L"GhostUp")
		AlphaBlend(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
			0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, _bf);
	else
		AlphaBlend(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, 
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, _bf);
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
		}
		m_ePreState = m_eCurState;
		m_bPreFlip = m_bCurFlip;
	}
}

void CGhost::Follow()
{
	INFO _playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	float _playerScreenX = _playerInfo.fX + CScrollMgr::Get_Instance()->Get_ScrollX();
	float _playerScreenY = _playerInfo.fY + CScrollMgr::Get_Instance()->Get_ScrollY();

	if (_playerScreenX - 10.f < m_tInfo.fX && m_tInfo.fX < _playerScreenX + 10.f)
	{
		m_bFlip = false;
		m_pFrameKey = L"GhostUp";
		if (_playerScreenY - 5.f < m_tInfo.fY && m_tInfo.fY < _playerScreenY + 5.f)
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHp(0);
		}
		else if (_playerScreenY < m_tInfo.fY)
			m_tInfo.fY -= m_fSpeed * 0.5f;
		else
			m_tInfo.fY += m_fSpeed * 0.5f;


	}
	else if (_playerScreenX < m_tInfo.fX)
	{
		m_bFlip = true;
		m_pFrameKey = L"GhostFlip";
		m_tInfo.fX -= m_fSpeed;

		if (_playerScreenY < m_tInfo.fY)
			m_tInfo.fY -= m_fSpeed * 0.5f;
		else
			m_tInfo.fY += m_fSpeed * 0.5f;
	}
	else if (_playerScreenX > m_tInfo.fX)
	{	
		m_bFlip = false;
		m_pFrameKey = L"Ghost";
		m_tInfo.fX += m_fSpeed;

		if (_playerScreenY < m_tInfo.fY)
			m_tInfo.fY -= m_fSpeed * 0.5f;
		else
			m_tInfo.fY += m_fSpeed * 0.5f;
	}
}
