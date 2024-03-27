#include "stdafx.h"
#include "Box.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "Rope.h"

CRope::CRope()
{
	m_eMyObjType = OBJECT_TYPE::ROPE;
}

CRope::~CRope()
{
	Release();
}

void CRope::Initialize()
{
	m_tInfo = { 0, 0, TILECX, TILECY};
	m_fSpeed = 3.f;
	m_pFrameKey = L"Rope";
	m_eRender = RENDER_GAMEOBJECT;
}

int CRope::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CRope::Late_Update()
{
	//if (Gravity() == true)
	//	CLineMgr::Get_Instance()->Box_Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY);

	//m_boxLine->Set_Info(LINE(LINEPOINT{ m_tInfo.fX - TILECX * 0.5f + 1.f , m_tInfo.fY - TILECY * 0.5f - 0.01f }, LINEPOINT{ m_tInfo.fX + TILECX * 0.5f - 1.f, m_tInfo.fY - TILECY * 0.5f - 0.01f }));
}

void CRope::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	
	for(int i = 0; i <= m_iRopeSize; i++)
		Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + TILECY * i + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + TILECY * i + iScrollY);

	//HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	hMemDC, 0, 0, TILECX, TILECY, RGB(255, 255, 255));
}

void CRope::Release()
{
}
