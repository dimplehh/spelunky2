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
	HDC	hMemDC = NULL;

	if (m_iOption == 1 || m_iOption == 2)
	{
		int idx = (CSceneMgr::Get_Instance()->GetRealScene()->GetMapNum() - 1) * 2 + m_iOption;
		wstring wStr = L"Tile" + to_wstring(idx);
		const wchar_t* name = wStr.c_str();

		m_pFrameKey = name;

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	}

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, TILECX, TILECY,
		hMemDC, TILECX * m_iDrawID, 0, TILECX, TILECY, RGB(255, 255, 255));
}

void CTile::Release()
{
}
