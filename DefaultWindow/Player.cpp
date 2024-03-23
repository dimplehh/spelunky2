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
#include <iostream>


CPlayer::CPlayer()
	: m_fDistance(0.f), m_bJump(false), m_bLadder(false), m_iJumpCount(0), m_iHp(100), m_fPreY(0.f), m_fCurY(0.f),
	m_fTime(0.f), m_fPower(0.f), m_ePreState(ST_END), m_eCurState(IDLE), m_bKneelDown(false), m_bAttachedBox(false), m_dwTime(GetTickCount())
{
	ZeroMemory(&m_tPosin, sizeof(POINT));

	m_eMyObjType = OBJECT_TYPE::PLAYER;
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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_base2.bmp",  L"Player_BASE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_flip2.bmp", L"Player_FLIP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_Attack.bmp", L"Player_ATTACK_BASE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_Attack_Flip.bmp", L"Player_ATTACK_FLIP");

	m_eCurState = IDLE;
	m_tFrame = { 0, 0, 0, 15, false, 60, GetTickCount(), 0};
	m_pFrameKey = L"Player_BASE";

	m_eRender = RENDER_GAMEOBJECT;
}

int CPlayer::Update()
{
	Die();
	Key_Input();

	Offset();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	Gravity();
	CheckFall();
	CheckAlmostFell();
	CheckCanHanging();
	Motion_Change();
	__super::Move_Frame();
#ifdef _DEBUG

	if (m_dwTime + 1000 < GetTickCount())
	{
		cout << "플레이어 좌표 : " << m_tInfo.fX << "\t" << m_tInfo.fY << endl;
		m_dwTime = GetTickCount();
	}
#endif
}

void CPlayer::Render(HDC hDC)
{
	SetRenderImage(hDC);
}

void CPlayer::SetRenderImage(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_eCurState == ATTACK)
	{
		if (m_bFlip == false)	m_pFrameKey = L"Player_ATTACK_BASE";
		else					m_pFrameKey = L"Player_ATTACK_FLIP";

		HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX - 32.f, m_tRect.top + iScrollY, 128, (int)m_tInfo.fCY,
			hMemDC, m_tFrame.iFrameStart * 128, m_tFrame.iMotion * (int)m_tInfo.fCY, 128, (int)m_tInfo.fCY, RGB(62, 62, 62));
	}
	else
	{
		if (m_bFlip == false)	m_pFrameKey = L"Player_BASE";
		else					m_pFrameKey = L"Player_FLIP";
		HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, m_tFrame.iMotion * (int)m_tInfo.fCY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(62, 62, 62));
	}
}

void CPlayer::Release()
{
}

void CPlayer::HoldLeft()
{
	if (m_bLadder)
		return;
	m_pFrameKey = L"Player_FLIP";
	m_bFlip = true;
	if (m_bKneelDown)
	{
		m_eCurState = CRAWL;
		m_tInfo.fX -= m_fSpeed * 0.25f;
	}
	else if (m_bAttachedBox)
	{
		m_eCurState = PUSH;
		m_tInfo.fX -= m_fSpeed * 0.25f;
	}
	else
	{
		if (!m_bJump && CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
		{
			m_eCurState = WALK;
		}
		m_tInfo.fX -= m_fSpeed;
	}
}

void CPlayer::HoldRight()
{
	m_pFrameKey = L"Player_BASE";
	m_bFlip = false;
	if (m_bKneelDown)
	{
		m_eCurState = CRAWL;
		m_tInfo.fX += m_fSpeed * 0.25f;
	}
	else if (m_bAttachedBox)
	{
		m_eCurState = PUSH;
		m_tInfo.fX += m_fSpeed * 0.25f;
	}
	else
	{
		if (m_bLadder)
			return;
		if (!m_bJump && CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
		{
			m_eCurState = WALK;
		}
		m_tInfo.fX += m_fSpeed;
	}
}

void CPlayer::HoldUp()
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

void CPlayer::HoldDown()
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

void CPlayer::TapZ()
{
	if (m_bKneelDown == true)	// 하향점프
	{
		if (CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump))
		{
			if (CLineMgr::Get_Instance()->LastBottom_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
			{
				m_tInfo.fY += 50;
			}
		}
		if (170.f < m_fAngle && m_fAngle < 270.f)
			m_fAngle += 10.f;
		else
			m_fAngle -= 10.f;
	}
	else
	{
		if (m_iJumpCount < 1) //무한점프 방지
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

bool CPlayer::Die()
{
	if (m_iHp < 0)
	{
		m_iHp = 0;
		m_eCurState = DIE;
		return true;
	}
	return false;
}

void CPlayer::CheckFall()
{
	m_fCurY = CLineMgr::Get_Instance()->GetY();
	if (m_fPreY != m_fCurY)
	{
		m_fDiffY = m_fCurY - m_fPreY;
		if (m_fDiffY > 200.f)
		{
			m_iHp -= 10;
			m_eCurState = FALLING;
		}
		m_fPreY = m_fCurY;
	}
	else
	{
		if(CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, m_bJump) && m_eCurState == FALLING)
			m_eCurState = DIZZY;
	}
}

void CPlayer::CheckAlmostFell()
{
	if (nullptr != CLineMgr::Get_Instance()->Get_AttachedLine())
	{
		if ((0 < (int)CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fX - m_tInfo.fX	&& (int)CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tLPoint.fX - m_tInfo.fX < 10)
		|| ( 0 < m_tInfo.fX - (int)CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tRPoint.fX	&& m_tInfo.fX - (int)CLineMgr::Get_Instance()->Get_AttachedLine()->Get_Info().tRPoint.fX < 10))
		{
			if(m_tInfo.fY )
			if (m_eCurState == IDLE)
				m_eCurState = ALMOSTFELL;
		}
	}
	else
	{
		if (m_eCurState == ALMOSTFELL)
			m_eCurState = IDLE;
	}
}

void CPlayer::CheckCanHanging()
{
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

bool CPlayer::Check_Move_End()
{
	if (m_tFrame.bRoop == false)
	{
		if (m_tFrame.iRepeat == m_iRepeatCount)
			return true;
	}
	return false;
}

void CPlayer::Offset()
{
	int		iOffSetminX = 390;
	int		iOffSetmaxX = 410;

	int		iOffSetminY = 290;
	int		iOffSetmaxY = 310;

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_eCurState == LOOKUP || m_eCurState == KNEELSTAY)
	{
		if (Check_Move_End() == true)
		{
			if (m_fMoveOffset < 15.f)
			{
				m_fMoveOffset += 0.8f;
				if(m_eCurState == LOOKUP)
					CScrollMgr::Get_Instance()->Set_ScrollY(m_fMoveOffset);
				else if (m_eCurState == KNEELSTAY)
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

void CPlayer::Gravity()	//숫자 의미 판단, 더 정리 필요 -> Obj로 나중에 빼야될듯
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

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		m_iRepeatCount = 0;
		switch (m_eCurState)
		{
		case CPlayer::IDLE:			Set_Frame(0, 0, 0, false, 60, 0, 15);		break;
		case CPlayer::WALK:			Set_Frame(1, 8, 0, true, 30, 0, 15);		break;
		case CPlayer::JUMP:			Set_Frame(0, 11, 9, false, 15, 1, 15);		break;
		case CPlayer::FALLING:		Set_Frame(0, 11, 9, false, 15, 1, 15);		break;
		case CPlayer::DIZZY:		Set_Frame(0, 11, 13, false, 60, 3, 15);		break;
		case CPlayer::DIE:			Set_Frame(9, 9, 0, false, 60, 1, 15);		break;
		case CPlayer::LOOKUP:		Set_Frame(0, 3, 8, false, 60, 0, 15);		break;
		case CPlayer::LOOKFRONT:	Set_Frame(3, 6, 8, false, 60, 1, 15);		break;
		case CPlayer::KNEELDOWN:	Set_Frame(0, 2, 1, false, 60, 1, 15);		break;
		case CPlayer::KNEELSTAY:	Set_Frame(2, 2, 1, false, 60,1, 15);		break;
		case CPlayer::CRAWL:		Set_Frame(5, 11, 1, true, 20, 0, 15);		break;
		case CPlayer::STANDUP:		Set_Frame(2, 4, 1, false, 60, 1, 15);		break;
		case CPlayer::ATTACKED:		Set_Frame(0, 11, 13, false, 60, 3, 15);		break;
		case CPlayer::ALMOSTFELL:	Set_Frame(0, 7, 3, true, 60, 0, 15);		break;
		case CPlayer::ATTACK:		Set_Frame(0, 7, 0, false, 60, 1, 7);		break;
		case CPlayer::ENTER:		Set_Frame(0, 5, 5, false, 30, 2, 15);		break;
		case CPlayer::EXIT:			Set_Frame(6, 11, 5, false, 30,2, 15);		break;
		case CPlayer::LADDER:		Set_Frame(0, 5, 6, true, 60 , 0, 15);		break;
		case CPlayer::PUSH:			Set_Frame(6, 11, 6, true, 60, 0, 15);		break;	// 수평선에선 잘 됨
		case CPlayer::HANGON:		Set_Frame(8, 11, 3, true, 100, 0, 15);		break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Key_Input()
{
	if (m_eCurState == DIE)
		return;

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD) { HoldLeft(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD) { HoldRight(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD) { HoldUp(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::AWAY) { if (!m_bLadder)		m_eCurState = LOOKFRONT; }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::TAP) { if (!m_bLadder) { m_eCurState = KNEELDOWN;	m_bKneelDown = true; } }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD) { HoldDown(); }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::AWAY) { if (!m_bLadder) { m_eCurState = STANDUP;		m_bKneelDown = false; } }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::A) == KEY_STATE::TAP) { m_iHp -= 10;		m_eCurState = ATTACKED; }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::E) == KEY_STATE::HOLD) { m_eCurState = ENTER; }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::E) == KEY_STATE::AWAY) { m_eCurState = EXIT; }
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::X) == KEY_STATE::TAP) { m_eCurState = ATTACK; }
	else if ((m_tFrame.bRoop == true && m_bLadder == false || ((m_tFrame.bRoop == false) && Check_Move_End() == true) && m_eCurState != FALLING))
	{
		m_eCurState = IDLE;
		m_bKneelDown = false;
	}
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::Z) == KEY_STATE::TAP) { TapZ(); }
}
