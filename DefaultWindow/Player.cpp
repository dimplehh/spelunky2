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
	: m_fDistance(0.f), m_bJump(false), m_fTime(0.f), m_fPower(0.f)
	, m_ePreState(ST_END), m_eCurState(IDLE)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo		= { 100.f, WINCY / 2.f, 200.f, 200.f };
	m_fSpeed	= 3.f;
	m_fDistance = 100.f;
	m_fPower = 20.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_DOWN.bmp",  L"Player_DOWN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_UP.bmp",	 L"Player_UP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LEFT.bmp",  L"Player_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RIGHT.bmp", L"Player_RIGHT");
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LU.bmp", L"Player_LU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RU.bmp", L"Player_RU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LD.bmp", L"Player_LD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RD.bmp", L"Player_RD");


	m_eCurState = IDLE;
	m_tFrame = { 0, 3, 0, 200, GetTickCount() };
	m_pFrameKey = L"Player_DOWN";

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
	Jump();
	
	Motion_Change();
	__super::Move_Frame();

}

void CPlayer::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	// 비트맵 출력 함수
	/*BitBlt(hDC,		// 최종적인 그림을 그릴 dc
		m_tRect.left + iScrollX,	// 복사 받을 위치의 left
		m_tRect.top,				// 복사 받을 위치의 top
		m_tInfo.fCX,				// 복사 받을 가로 길이
		m_tInfo.fCY,				// 복사 받을 세로 길이
		hMemDC,	// 비트맵을 가지고 있는 dc
		0,							// 출력할 비트맵의 시작 x좌표
		0,							// 출력할 비트맵의 시작 y좌표		
		SRCCOPY);					// 출력 효과(그래도 출력)
		*/

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		m_tRect.left + iScrollX, // 복사 받을 위치의 left
		m_tRect.top+ iScrollY,			 // 복사 받을 위치의 top
		(int)m_tInfo.fCX,			 // 복사 받을 가로 길이
		(int)m_tInfo.fCY,			 // 복사 받을 세로 길이
		hMemDC,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,						// 출력할 비트맵의 시작 x좌표
		m_tFrame.iMotion * (int)m_tInfo.fCY,						// 출력할 비트맵의 시작 y좌표
		(int)m_tInfo.fCX,			// 출력할 비트맵의 가로 사이즈
		(int)m_tInfo.fCY,			// 출력할 비트맵의 세로 사이즈
		RGB(0, 0, 0));	// 제거할 픽셀의 색상 값
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	float	fY(0.f);

	if (GetAsyncKeyState(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
		m_pFrameKey = L"Player_LEFT";
		m_eCurState = WALK;

	}

	else if (GetAsyncKeyState(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		m_pFrameKey = L"Player_RIGHT";
		m_eCurState = WALK;
	}

	else if (GetAsyncKeyState(VK_UP))
	{
		m_tInfo.fY -= m_fSpeed;
		m_pFrameKey = L"Player_UP";
		m_eCurState = WALK;

	}

	else if (GetAsyncKeyState(VK_DOWN))
	{
		m_tInfo.fY += m_fSpeed;
		m_pFrameKey = L"Player_DOWN";
		m_eCurState = WALK;
	}

	else if (CKeyMgr::Get_Instance()->Key_Up(VK_SPACE))
	{
		m_bJump = true;
	}
	else
		m_eCurState = IDLE;

}

void CPlayer::Jump()
{
	float	fY(0.f);

	bool	bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		m_tInfo.fY -= m_fPower * m_fTime - ((9.8f * m_fTime * m_fTime) * 0.5f);
		m_fTime += 0.2f;

		if (bLineCol && (fY < m_tInfo.fY))
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = fY;
		}

	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
	}
}

void CPlayer::Offset()
{
	/*int		iOffSetX = WINCX >> 1;

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	if ((float)iOffSetX > m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if ((float)iOffSetX < m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);*/


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

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		
		m_ePreState = m_eCurState;
	}
}
