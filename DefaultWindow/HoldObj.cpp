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

	if(m_bCollision == true	&& dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->GetIsHold() == true)
		SetOwner(CObjMgr::Get_Instance()->Get_Player());

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CHoldObj::Late_Update()
{
	if(m_pOwner == nullptr)
		Gravity();
	else
	{
		if (dynamic_cast<CPlayer*>(m_pOwner)->GetThrow() == false)
		{
			m_tInfo.fX = (m_pOwner)->Get_Info().fX;
			m_tInfo.fY = (m_pOwner)->Get_Info().fY  + 7.f;
		}
		else
		{
			if (!CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
			{
				CLineMgr::Get_Instance()->Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX , m_tInfo.fCY);

				if (dynamic_cast<CPlayer*>(m_pOwner)->GetFlip() == true)
					m_tInfo.fX -= 10.f;
				else
					m_tInfo.fX += 10.f;

				m_tInfo.fY = dynamic_cast<CPlayer*>(m_pOwner)->Get_Info().fY - 40.f * m_fTime + ((9.8f * m_fTime * m_fTime) * 0.5f);
				m_fTime += 0.5f;
			}
			else
			{
				dynamic_cast<CPlayer*>(m_pOwner)->SetIsHold(false);
				dynamic_cast<CPlayer*>(m_pOwner)->SetThrow(false);
				m_bDead = true;
			}
		}
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

void CHoldObj::Release()	//�׾Ƹ��� ��� ������ �� or ������ ���;���
{
	CSoundMgr::Get_Instance()->PlaySound(L"Splash.wav", SOUND_EFFECT, g_fVolume);
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CItemFactory::Create(m_tInfo.fX, m_tInfo.fY - 10.f, CItem::ITEM_GOLD));
}

bool CHoldObj::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Box_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
	{						//if �÷��̾ �浹���°� �ƴ϶�� + (�������°� �ƴ϶��)
		m_fTime += 0.2f;

		float fGravity = m_fPower * m_fTime - ((4.9f * m_fTime * m_fTime) * 0.5f); // �߷� ���� 2�� �˻��ϱ⿡ �߷°� ����

		if (fGravity < -5.f)	// �߷� �ӵ��� �ִ�ġ ����
			fGravity = -5.f;

		if (fGravity > 5.f)	// ���� �ӵ��� �ִ�ġ ����
			fGravity = 5.f;

		m_tInfo.fY -= fGravity;	// y ���� �߷°���ŭ ��ȯ
		return false;
	}
	else	// �÷��̾ �浹 ���ΰ��
	{
		m_fPower = 0.f;			// ������ �ʱ�ȭ
		return true;
	}
}