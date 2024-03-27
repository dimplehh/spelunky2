#include "stdafx.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"
#include "TileMgr.h"
#include "Bomb.h"

CBomb::CBomb()
{
	m_eMyObjType = OBJECT_TYPE::BOMB;
}

CBomb::~CBomb()
{
	Release();
}

void CBomb::Initialize()
{
	m_tInfo = { 0, 0, TILECX, TILECY };
	m_fSpeed = 3.f;								//speed ��ſ� second.. ���� 3�� �ڿ� �������� ����
	m_pFrameKey = L"Bomb";
	m_eRender = RENDER_GAMEOBJECT;

	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();
}

int CBomb::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBomb::Late_Update()
{
	Gravity();
}

void CBomb::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	//HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	hMemDC, 0, 0, TILECX, TILECY, RGB(255, 255, 255));
}

void CBomb::Explosion()  //������	���� �ΰ��Ӱ� �Ȱ��� �����ϱ�
{
	int index = CTileMgr::Get_Instance()->Get_Tile_Idx(m_tInfo.fX, m_tInfo.fY);

	(*m_pVecTile)[index - 2]->Set_Option(0);
	(*m_pVecTile)[index - 2]->Set_DrawID(0);
	(*m_pVecTile)[index - 1]->Set_Option(0);
	(*m_pVecTile)[index - 1]->Set_DrawID(0);
	(*m_pVecTile)[index]->Set_Option(0);
	(*m_pVecTile)[index]->Set_DrawID(0);
	(*m_pVecTile)[index + 1]->Set_Option(0);
	(*m_pVecTile)[index + 1]->Set_DrawID(0);
	(*m_pVecTile)[index + 2]->Set_Option(0);
	(*m_pVecTile)[index + 2]->Set_DrawID(0);

	CLineMgr::Get_Instance()->Release();
	CLineMgr::Get_Instance()->SetLine(); //���� �缼��

	m_bDead = true;
}

bool CBomb::Gravity()
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
void CBomb::Release()
{
}