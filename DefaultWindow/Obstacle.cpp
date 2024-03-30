#include "stdafx.h"
#include "Obstacle.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"

CObstacle::CObstacle()
{
	m_eMyObjType = OBJECT_TYPE::OBSTACLE;
}

CObstacle::~CObstacle()
{
	Release();
}

void CObstacle::Initialize()
{
	m_tInfo = { 0, 0, TILECX, TILECY };
	m_pFrameKey = L"Obstacle";
	m_eRender = RENDER_GAMEOBJECT;
}

int CObstacle::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CObstacle::Late_Update()
{
	INFO _playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	if (m_tInfo.fX - m_tInfo.fCX / 2 + 2.f <= _playerInfo.fX && _playerInfo.fX <= m_tInfo.fX + m_tInfo.fCX / 2 - 2.f
		&& m_tInfo.fY - m_tInfo.fCY / 2 + 20.f <= _playerInfo.fY && _playerInfo.fY <= m_tInfo.fY + m_tInfo.fCY / 2)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHp(0);
		m_iMotion = 1;
	}
	else
		m_iMotion = 0;
}

void CObstacle::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_iMotion * TILECX, 0, TILECX, TILECY, RGB(55, 55, 55));
}

void CObstacle::Release()
{
}
