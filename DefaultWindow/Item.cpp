#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "Item.h"

CItem::CItem() : m_fTime(0.f), m_fPower(0.f)
{
	m_eMyObjType = OBJECT_TYPE::ITEM;
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_tInfo = { 0, 0, 48.f , 48.f };
	m_pFrameKey = L"Item";
	m_eRender = RENDER_GAMEOBJECT;
}

int CItem::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CItem::Late_Update()
{
	Gravity();
}

void CItem::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, m_eItemID * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(38, 38, 38));
}

void CItem::Release()
{
}

bool CItem::Gravity()
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