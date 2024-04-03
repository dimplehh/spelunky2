#include "stdafx.h"
#include "Monster.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"

CMonster::CMonster() : m_fTime(0.f), m_fPower(0.f), m_iHp(1), m_iAttackPower(1), m_eMonsterID(MONSTER_END)
{
	m_eMyObjType = OBJECT_TYPE::MONSTER;
}

CMonster::~CMonster()
{
}

void CMonster::Gravity()
{
	if (!CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, false))
	{						//if �÷��̾ �浹���°� �ƴ϶�� + (�������°� �ƴ϶��)
		m_fTime += 0.2f;

		float fGravity = m_fPower * m_fTime - ((5.1f * m_fTime * m_fTime) * 0.5f); // �߷� ���� 2�� �˻��ϱ⿡ �߷°� ����

		if (fGravity < -5.f)	// �߷� �ӵ��� �ִ�ġ ����
			fGravity = -5.f;

		if (fGravity > 5.f)	// ���� �ӵ��� �ִ�ġ ����
			fGravity = 5.f;

		m_tInfo.fY -= fGravity;	// y ���� �߷°���ŭ ��ȯ
	}
	else	// �÷��̾ �浹 ���ΰ��
	{
		m_fPower = 0.f;			// ������ �ʱ�ȭ
		return;
	}
}
