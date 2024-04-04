#include "stdafx.h"
#include "Box.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"

CBox::CBox() : m_fTime(0.f), m_fPower(0.f), m_dwTime(GetTickCount())
{
	m_eMyObjType = OBJECT_TYPE::BOX;
}

CBox::~CBox()
{
	Release();
}

void CBox::Initialize()
{
	m_tInfo = { 0, 0, TILECX - 1, TILECY - 1};
	m_fSpeed = 3.f;
	m_pFrameKey = L"Box";
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
	if(Gravity() == true)
		CLineMgr::Get_Instance()->Box_Collision_Vertical_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY);

	m_boxLine->Set_Info(LINE(LINEPOINT{ m_tInfo.fX - TILECX * 0.49f , m_tInfo.fY - TILECY * 0.5f + 0.5f }, LINEPOINT{ m_tInfo.fX + TILECX * 0.49f, m_tInfo.fY - TILECY * 0.5f + 0.5f }));
}

void CBox::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, TILECX, TILECY,
		hMemDC, 0, 0, TILECX, TILECY, RGB(255, 255, 255));
}

void CBox::Release()
{
}

bool CBox::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Box_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
	{						//if �÷��̾ �浹���°� �ƴ϶�� + (�������°� �ƴ϶��)
		m_fTime += 0.2f;

		float fGravity = m_fPower * m_fTime - ((5.f * m_fTime * m_fTime) * 0.5f); // �߷� ���� 2�� �˻��ϱ⿡ �߷°� ����

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