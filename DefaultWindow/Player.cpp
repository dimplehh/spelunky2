#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "AbstractFactory.h"
#include "Shield.h"
#include "ScrewBullet.h"
#include "ObjMgr.h"
#include "GuideBullet.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"


CPlayer::CPlayer()
	: m_fDistance(0.f), m_bJump(false), m_bLadder(false), m_iJumpCount(0),
	m_fTime(0.f), m_fPower(0.f), m_ePreState(ST_END), m_eCurState(IDLE), m_bKneelDown(false)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo		= { WINCX / 2.f, WINCY / 2.f, 64.f, 64.f };
	m_fSpeed	= 5.f;
	m_fDistance = 100.f;
	m_fPower = 6.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_base.bmp",  L"Player_BASE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_flip.bmp", L"Player_FLIP");

	m_eCurState = IDLE;
	m_tFrame = { 0, 0, 0, 15, false, 60, GetTickCount()};
	m_pFrameKey = L"Player_BASE";

	m_eRender = RENDER_GAMEOBJECT;
}

int CPlayer::Update()
{
	Key_Input();
	Offset();
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	Gravity();
	Motion_Change();
	__super::Move_Frame();

}

void CPlayer::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top+ iScrollY, 
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,	
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,	RGB(62, 62, 62));
	TCHAR	szBuff[80] = L"";
	//swprintf_s(szBuff, L"start frame,end frame, 줄 번호: %d, %d, %d",m_tFrame.iFrameStart, m_tFrame.iFrameEnd, m_tFrame.iMotion);
	if (nullptr == CLineMgr::Get_Instance()->Get_AttachedLine())
	{
		swprintf_s(szBuff, L"선 양끝점:  %f, %f", CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint,
			CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tRPoint);
	}
	TextOut(hDC, 50, 100, szBuff, lstrlen(szBuff));
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()	// 이 코드 자체를 좀 깔끔히 정리 필요
{
	float	fY(0.f);

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		m_pFrameKey = L"Player_FLIP";
		m_bFlip = true;
		if (!m_bLadder) 
		{
			if (m_bKneelDown)
			{
				m_eCurState = CRAWL;
				m_tInfo.fX -= m_fSpeed * 0.25f;
			}
			else
			{
				if(!m_bJump)
					m_eCurState = WALK;
				m_tInfo.fX -= m_fSpeed;
			}
		}
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		m_pFrameKey = L"Player_BASE";
		m_bFlip = false;
		if (!m_bLadder)
		{
			if (m_bKneelDown)
			{
				m_eCurState = CRAWL;
				m_tInfo.fX += m_fSpeed * 0.25f;
			}
			else
			{
				if (!m_bJump)
					m_eCurState = WALK;
				m_tInfo.fX += m_fSpeed;
			}
		}
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		InLadder();
		if (m_bLadder)
		{
			m_tInfo.fY -= m_fSpeed;
			m_eCurState = LADDER;
		}
		else
			m_eCurState = LOOKUP;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::AWAY)
	{
		if (!m_bLadder)
			m_eCurState = LOOKFRONT;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::TAP)
	{
		if (!m_bLadder)
		{
			m_eCurState = KNEELDOWN;
			m_bKneelDown = true;
		}
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
	{
		InLadder();
		if (m_bLadder)
		{
			m_tInfo.fY += m_fSpeed;
			m_eCurState = LADDER;
		}
		else
			m_eCurState = KNEELSTAY;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::AWAY)
	{
		if (!m_bLadder)
		{
			m_eCurState = STANDUP;
			m_bKneelDown = false;
		}
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::E) == KEY_STATE::HOLD)
	{
		m_eCurState = ENTER;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::E) == KEY_STATE::AWAY)
	{
		m_eCurState = EXIT;
	}
	else if (m_tFrame.bRoop == true && m_bLadder == false || m_tFrame.bRoop == false && Check_Move_End() == true && m_bKneelDown == false)
		m_eCurState = IDLE;

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::Z) == KEY_STATE::TAP)
	{
		if (m_bKneelDown == true)	// 하향점프
		{
			if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
			{
				if (CLineMgr::Get_Instance()->LastBottom_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
				{
					m_tInfo.fY += m_tInfo.fCY / 3.f;
				}
			}
			if (170.f < m_fAngle && m_fAngle < 270.f)
				m_fAngle += 10.f;
			else
				m_fAngle -= 10.f;
		}
		else
		{
			if (m_iJumpCount < 1)
			{
				m_bJump = true;
				m_eCurState = JUMP;
				m_fTime = 0.f;
				m_fPower = 10.f;
				m_bLadder = false;
				m_iJumpCount++;
			}
		}
	}
}

void CPlayer::Offset()
{
	int		iOffSetminX = 390;
	int		iOffSetmaxX = 410;

	int		iOffSetminY = 290;
	int		iOffSetmaxY = 310;

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_eCurState == LOOKUP)
	{
		if (Check_Move_End() == true)
		{
			if (m_fMoveOffset < 15.f)
			{
				m_fMoveOffset += 0.8f;
				CScrollMgr::Get_Instance()->Set_ScrollY(m_fMoveOffset);
			}
		}
	}
	else if (m_eCurState == KNEELSTAY)
	{
		if (Check_Move_End() == true)
		{
			if (m_fMoveOffset < 15.f)
			{
				m_fMoveOffset += 0.8f;
				CScrollMgr::Get_Instance()->Set_ScrollY(-m_fMoveOffset);
			}
		}
	}
	else
	{
		m_fMoveOffset = 0.f;
		if ((float)iOffSetminX > m_tInfo.fX + fScrollX)
			CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

		if ((float)iOffSetmaxX < m_tInfo.fX + fScrollX)
			CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

		if ((float)iOffSetminY > m_tInfo.fY + fScrollY)
			CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

		if ((float)iOffSetmaxY < m_tInfo.fY + fScrollY)
			CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);
	}
}

void CPlayer::Gravity()	//숫자 의미 판단, 더 정리 필요
{
	for (int i = 0; i < 2; i++)
	{
		if (!m_bLadder && !CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
		{
			m_fTime += 0.1f;

			float fGravity = m_fPower * m_fTime - ((4.9f * m_fTime * m_fTime) * 0.5f);

			if (fGravity < -5.f)
			{
				fGravity = -5.f;
				m_bJump = false;
			}
			else if (fGravity > 5.f)
				fGravity = 5.f;
			if (fGravity < 0)
				m_bJump = false;

			m_tInfo.fY -= fGravity;
		}
		else
		{
			m_fPower = 0.f;
			m_iJumpCount = 0;
			return;
		}
	}
}

void CPlayer::InLadder()
{
	if (CLineMgr::Get_Instance()->Ladder_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
	{
		m_iJumpCount = 0;
		m_bLadder = true;
	}
	else
		m_bLadder = false;
}

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			Set_Frame(0, 0, 0, false, 60);
			break;
		case CPlayer::WALK:
			Set_Frame(1, 8, 0, true, 30);
			break;
		case CPlayer::JUMP:
			Set_Frame(0, 11, 9, false, 15);
			break;
		case CPlayer::DIZZY:
			Set_Frame(9, 9, 0, true, 60);
			break;
		case CPlayer::DIE:
			Set_Frame(9, 9, 0, false, 60);
			break;
		case CPlayer::LOOKUP:
			Set_Frame(0, 3, 8, false, 60);
			break;
		case CPlayer::LOOKFRONT:
			Set_Frame(3, 6, 8, false, 60);
			break;
		case CPlayer::KNEELDOWN:
			Set_Frame(0, 2, 1, false, 60);
			break;
		case CPlayer::KNEELSTAY:
			Set_Frame(2, 2, 1, false, 60);
			break;
		case CPlayer::CRAWL:
			Set_Frame(5, 11, 1, true, 20);
			break;
		case CPlayer::STANDUP:
			Set_Frame(2, 4, 1, false, 60);
			break;
		case CPlayer::ATTACKED:
			Set_Frame(0, 3, 2, false, 60);
			break;
		case CPlayer::ALMOSTFELL:
			Set_Frame(0, 7, 3, true, 60);
			break;
		case CPlayer::ATTACK:
			Set_Frame(0, 10, 4, false, 60);
			break;
		case CPlayer::ENTER:
			Set_Frame(0, 5, 5, false, 30);
			break;
		case CPlayer::EXIT:
			Set_Frame(6, 11, 5, false, 30);
			break;
		case CPlayer::LADDER:
			Set_Frame(0, 5, 6, true, 60);
			break;
		case CPlayer::PUSH:
			Set_Frame(6, 11, 3, true, 60);
			break;
		}
		m_ePreState = m_eCurState;
	}
}
