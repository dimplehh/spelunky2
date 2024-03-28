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
	m_tFrame = { 5, 9, 0, 10, false, 60, GetTickCount(), 0 };
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
	__super::Move_Frame();
	CheckMoveEnd();
}

void CRope::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//맨 위 이미지
	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		3 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(62, 62, 62));

	for (int i = 1; i <= m_iRenderIndex; i++) // 줄 이미지로 고정되는 부분
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + TILECY * i + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
			9 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(62, 62, 62));

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + TILECY * m_iRenderIndex + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(62, 62, 62));
}

void CRope::Release()
{
}

void CRope::CheckMoveEnd()
{
	if (m_iRenderIndex >= m_iRopeSize)
		return;

	if (m_tFrame.bRoop == false)
	{
		if (m_tFrame.iRepeat == m_iRepeatCount)
			m_iRenderIndex++;
	}
}