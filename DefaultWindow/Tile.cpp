#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"


CTile::CTile() : m_iDrawID(0), m_iOption(0)
{
	
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_pFrameKey = L"Tile";
	m_eRender = RENDER_TILE;
}

int CTile::Update()
{

	__super::Update_Rect();

	return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC hDC)
{
	switch (CSceneMgr::Get_Instance()->GetRealScene()->GetMapNum())
	{
	case 1:
		if (m_iOption == 1) m_pFrameKey = L"Tile";
		else if (m_iOption == 2) m_pFrameKey = L"Tile2";
		break;
	case 2:
		if (m_iOption == 1) m_pFrameKey = L"Tile3";
		else if (m_iOption == 2) m_pFrameKey = L"Tile4";
		break;
	//case 3:
	//	if (m_iOption == 1) m_pFrameKey = L"Tile";
	//	else if (m_iOption == 2) m_pFrameKey = L"Tile2";
	//	break;
	default:
		break;
	}
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, TILECX, TILECY,
		hMemDC, TILECX * m_iDrawID, 0, TILECX, TILECY, RGB(255, 255, 255));
}

void CTile::Release()
{
}
