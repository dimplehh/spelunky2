#pragma once
#include "Obj.h"
#include "Player.h"
class CMonster : public CObj
{
public:
	enum MONSTERID {MONSTER_SNAKE, MONSTER_GHOST, MONSTER_END};

public:
	CMonster();
	virtual ~CMonster();

public:
	int		GetAttackPower() { return m_iAttackPower; }
	void	SetMonsterID(MONSTERID _monsterID) { m_eMonsterID = _monsterID; }

protected:
	int			m_iAttackPower;		// ���ݷ�
	int			m_iHp;				// ü��

	void		Gravity();

	float		m_fPower;
	float		m_fTime;

	MONSTERID	m_eMonsterID;
};
