#pragma once
#include "Obj.h"
#include "Player.h"
class CMonster : public CObj
{
public:
	enum MONSTERID {SNAKE, GHOST, MONSTER_END};

public:
	CMonster();
	virtual ~CMonster();

public:
	int			GetAttackPower() { return m_iAttackPower; }
	void		SetMonsterID(MONSTERID _monsterID) { m_eMonsterID = _monsterID; }
	MONSTERID	GetMonsterID() { return m_eMonsterID; }

protected:
	int			m_iAttackPower;		// 공격력
	int			m_iHp;				// 체력

	void		Gravity();

	float		m_fPower;
	float		m_fTime;

	MONSTERID	m_eMonsterID;
};
