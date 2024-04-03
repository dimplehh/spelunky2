#include "stdafx.h"
#include "Lava.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CLava::CLava()
{
	m_eMyObjType = OBJECT_TYPE::LAVA;
}

CLava::~CLava()
{
	Release();
}

void CLava::Initialize()
{
	m_tInfo = { 0, 0, TILECX  *10, TILECY * 3};
	m_pFrameKey = L"Lava";
	m_eRender = RENDER_GAMEOBJECT;
}

int CLava::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CLava::Late_Update()
{
}

void CLava::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC,0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(0, 0, 0));
}

void CLava::Release()
{
}
