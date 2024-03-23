#include "stdafx.h"
#include "Box.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"

CBox::CBox() : m_fTime(0.f), m_fPower(0.f)
{
	m_eMyObjType = OBJECT_TYPE::BOX;
}

CBox::~CBox()
{
	Release();
}

void CBox::Initialize()
{
	m_tInfo = { WINCX * 0.5f, WINCY * 0.5f, 50.f, 50.f };
	m_fSpeed = 3.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CBox::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBox::Late_Update()
{
	Gravity();
}

void CBox::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
}

void CBox::Release()
{
}

void CBox::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
	{						//if 플레이어가 충돌상태가 아니라면 + (점프상태가 아니라면)
		m_fTime += 0.2f;

		float fGravity = m_fPower * m_fTime - ((4.9f * m_fTime * m_fTime) * 0.5f); // 중력 적용 2번 검사하기에 중력값 절반

		if (fGravity < -5.f)	// 중력 속도의 최대치 설정
			fGravity = -5.f;

		if (fGravity > 5.f)	// 점프 속도의 최대치 설정
			fGravity = 5.f;

		m_tInfo.fY -= fGravity;	// y 값은 중력값만큼 변환
	}
	else	// 플레이어가 충돌 중인경우
	{
		m_fPower = 0.f;			// 점프력 초기화
		return;
	}
}