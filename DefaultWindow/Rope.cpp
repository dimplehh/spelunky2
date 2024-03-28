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
	//for(int i = 0; i <= m_iRopeSize; i++)
	//	Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + TILECY * i + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + TILECY * i + iScrollY);

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	for (int i = 0; i <= m_iRenderIndex; i++) // 줄 이미지로 고정되는 부분
		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + TILECY * i + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC,
			9 * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(62, 62, 62));

	//줄 애니메이션이 동작하는 부분
	//if(m_iRenderIndex == 0)	
	//	m_tFrame = { 0, 3, 0, 10, false, 60, GetTickCount(), 0 };
	//else
	//	m_tFrame = { 5, 9, 0, 10, false, 60, GetTickCount(), 0 };

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