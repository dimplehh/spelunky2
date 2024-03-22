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
	: m_fDistance(0.f), m_bJump(false), 
	m_fTime(0.f), m_fPower(0.f), m_ePreState(ST_END), m_eCurState(IDLE)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo		= { 100.f, WINCY / 2.f, 64.f, 64.f };
	m_fSpeed	= 5.f;
	m_fDistance = 100.f;
	m_fPower = 10.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_base.bmp",  L"Player_BASE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Crump_flip.bmp", L"Player_FLIP");

	m_eCurState = IDLE;
	m_tFrame = { 15, 0, 0, 0, false, 60, GetTickCount()};
	m_pFrameKey = L"Player_BASE";

	m_eRender = RENDER_GAMEOBJECT;
}

int CPlayer::Update()
{
	Key_Input();
	Offset();
	__super::Update_Rect();

	Load_Line();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
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
	TCHAR	szBuff[50] = L"";
	swprintf_s(szBuff, L"flip, 프레임,end frame, 줄 번호: %d, %d, %d, %d", m_bFlip, m_tFrame.iFrameStart, m_tFrame.iFrameEnd, m_tFrame.iMotion);
	TextOut(hDC, 50, 100, szBuff, lstrlen(szBuff));
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input() //이거 순서 생각 잘 해야됨. (여기 안에다 멤버함수로 분리하기)
{
	float	fY(0.f);

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		m_pFrameKey = L"Player_FLIP";
		m_bFlip = true;
		if(!m_bJump) m_eCurState = WALK;
		m_tInfo.fX -= m_fSpeed;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		m_pFrameKey = L"Player_BASE";
		m_bFlip = false;
		if (!m_bJump) m_eCurState = WALK;
		m_tInfo.fX += m_fSpeed;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		m_eCurState = LOOKUP;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::UP) == KEY_STATE::AWAY)
	{
		m_eCurState = LOOKFRONT;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
	{
		m_eCurState = KNEELDOWN;
	}
	else if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::DOWN) == KEY_STATE::AWAY)
	{
		m_eCurState = STANDUP;
	}
	else if (m_tFrame.bRoop == true || m_tFrame.bRoop == false && Check_Move_End() == true)
		m_eCurState = IDLE;
	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::Z) == KEY_STATE::TAP)
	{
		m_bJump = true;
		m_eCurState = JUMP;
	}
}

void CPlayer::Offset()
{
	int		iOffSetminX = 100;
	int		iOffSetmaxX = 700;

	int		iOffSetminY = 100;
	int		iOffSetmaxY = 500;

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if ((float)iOffSetminX > m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if ((float)iOffSetmaxX < m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

	if ((float)iOffSetminY > m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if ((float)iOffSetmaxY < m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);
}

void CPlayer::Motion_Change() //일단 싹다 세팅해놓기
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			Set_Frame(15, 0, 0, 0, false, 60);
			break;

		case CPlayer::WALK:
		{
			Set_Frame(15, 1, 8, 0, true, 30);
			break;
		}
		case CPlayer::JUMP:
			Set_Frame(15, 0, 11, 9, false, 15);
			break;

		case CPlayer::DIZZY:
			Set_Frame(15, 9, 9, 0, true, 60);
			break;

		case CPlayer::DIE:
			Set_Frame(15, 9, 9, 0, false, 60);
			break;

		case CPlayer::LOOKUP:
			Set_Frame(14, 0, 3, 8, false, 60);
			break;

		case CPlayer::LOOKFRONT:
			Set_Frame(14, 3, 6, 8, false, 60);
			break;

		case CPlayer::KNEELDOWN:
			Set_Frame(11, 0, 2, 1, false, 60);
			break;

		case CPlayer::CRAWL:
			Set_Frame(11, 5, 11, 1, true, 60);
			break;

		case CPlayer::STANDUP:
			Set_Frame(11, 2, 4, 1, false, 60);
			break;

		case CPlayer::GRAB:
			Set_Frame(11, 5, 11, 2, false, 60);
			break;

		case CPlayer::ATTACKED:
			Set_Frame(11, 0, 3, 2, false, 60);
			break;

		case CPlayer::ALMOSTFELL:
			Set_Frame(11, 0, 7, 3, true, 60);
			break;

		case CPlayer::ATTACK:
			Set_Frame(10, 0, 10, 4, false, 60);
			break;

		case CPlayer::ENTER:
			Set_Frame(11, 0, 5, 5, true, 60);
			break;

		case CPlayer::EXIT:
			Set_Frame(11, 6, 11, 5, true, 60);
			break;

		case CPlayer::LADDER:
			Set_Frame(11, 0, 5, 6, true, 60);
			break;

		case CPlayer::PUSH:
			Set_Frame(11, 6, 11, 3, true, 60);
			break;
		}
		
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Load_Line()
{
	float	fY(0.f);

	bool	bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		m_tInfo.fY -= m_fPower * m_fTime - ((9.8f * m_fTime * m_fTime) * 0.5f);
		m_fTime += 0.1f;

		if (bLineCol && (fY < m_tInfo.fY))
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = fY;
		}
	}
	else if (bLineCol)
	{
		m_bJump = false;
		m_tInfo.fY = fY;
	}
}