#include "stdafx.h"
#include "HoldObj.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"

extern float g_fVolume;

CHoldObj::CHoldObj() : m_fTime(0.f), m_fPower(0.f), m_eHoldObjID(HOLDOBJ_END), m_dwTime(GetTickCount()), m_fThrowTime(0.f)
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
	m_fTime = 0.f;
	m_eRender = RENDER_HOLDOBJ;
}

int CHoldObj::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

// 중력 적용을 받지 않을 때 : 플레이어가 들고 있을 때, 포물선 그려서 던져지다가 지면에 착지하기 전까지
// 중력 적용을 받을 때 : 플레이어가 들고있지 않을 때

void CHoldObj::Late_Update()
{
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetAttatchedBox() == true)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetCanHold(false);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetIsHold(false);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetThrow(false);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHoldObjIdx(-1);
		m_bCollision = false;
	}
	if ((dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetHoldObjIdx() != m_iIdx &&
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetCanHold() == false)
		|| dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetAttatchedBox() == true)
	{
		return;
	}

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetThrow() == true)
	{
		CLineMgr::Get_Instance()->Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY);

		if (!CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY))
		{
			if (!Gravity())
			{
				if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetFlip() == true)
					m_tInfo.fX -= 10.f;
				else
					m_tInfo.fX += 10.f;

				m_tInfo.fY = (CObjMgr::Get_Instance()->Get_Player())->Get_Info().fY - 40.f * m_fThrowTime + ((9.8f * m_fThrowTime * m_fThrowTime) * 0.5f);
				m_fThrowTime += 0.5f;
			}
		}
		else
		{
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetIsHold(false);
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetThrow(false);
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetHoldObjIdx(-1);
			m_bCollision = false;

			m_fThrowTime = 0.f;

			if (m_eHoldObjID == HOLDOBJ_JAR)
				m_bDead = true;
		}
	}
	else if (m_bCollision == true && dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetIsHold() == true)
	{
		m_tInfo.fX = (CObjMgr::Get_Instance()->Get_Player())->Get_Info().fX;
		m_tInfo.fY = (CObjMgr::Get_Instance()->Get_Player())->Get_Info().fY + 7.f;
	}
}

void CHoldObj::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + 10 + iScrollY, 48, 48,
		hMemDC, m_eHoldObjID * (int)m_tInfo.fCX, 0, m_tInfo.fCX, m_tInfo.fCY, RGB(255, 255, 255));
}

void CHoldObj::Release()	//항아리의 경우 던지면 깨지면서 금 or 보석이 나와야함
{
	CSoundMgr::Get_Instance()->PlaySound(L"Splash.wav", SOUND_EFFECT, g_fVolume);
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CItemFactory::Create(m_tInfo.fX, m_tInfo.fY - 10.f, CItem::ITEM_GOLD));
}

bool CHoldObj::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Box_Line(m_tInfo.fX, m_tInfo.fY, (m_tInfo.fCX - 15.f), (m_tInfo.fCY - 15.f), false))
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