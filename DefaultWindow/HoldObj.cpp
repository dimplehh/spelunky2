#include "stdafx.h"
#include "HoldObj.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CHoldObj::CHoldObj() : m_fTime(0.f), m_fPower(0.f), m_eHoldObjID(HOLDOBJ_JAR), m_pOwner(nullptr)
{
	m_eMyObjType = OBJECT_TYPE::HOLDOBJ;
}

CHoldObj::~CHoldObj()
{
	Release();
}

void CHoldObj::Initialize()
{
	m_tInfo = { 0, 0, 48.f , 48.f };
	m_pFrameKey = L"HoldObj";
	m_eRender = RENDER_GAMEOBJECT;
}

int CHoldObj::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CHoldObj::Late_Update()
{
	if(m_pOwner == nullptr)
		Gravity();
	else
	{
		m_tInfo.fX = (m_pOwner)->Get_Info().fX;
		m_tInfo.fY = (m_pOwner)->Get_Info().fY;
	}
}

void CHoldObj::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + 20 + iScrollY, 48, 48,
		hMemDC, m_eHoldObjID * (int)m_tInfo.fCX, 0, m_tInfo.fCX, m_tInfo.fCY, RGB(255, 255, 255));
}

void CHoldObj::Release()	//항아리의 경우 던지면 보석이 나와야함
{
}

bool CHoldObj::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Box_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
	{						//if 플레이어가 충돌상태가 아니라면 + (점프상태가 아니라면)
		m_fTime += 0.2f;

		float fGravity = m_fPower * m_fTime - ((4.9f * m_fTime * m_fTime) * 0.5f); // 중력 적용 2번 검사하기에 중력값 절반

		if (fGravity < -5.f)	// 중력 속도의 최대치 설정
			fGravity = -5.f;

		if (fGravity > 5.f)	// 점프 속도의 최대치 설정
			fGravity = 5.f;

		m_tInfo.fY -= fGravity;	// y 값은 중력값만큼 변환
		return false;
	}
	else	// 플레이어가 충돌 중인경우
	{
		m_fPower = 0.f;			// 점프력 초기화
		return true;
	}
}