#pragma once
#include "Monster.h"

class CGhost : public CMonster
{
public:
	enum STATE { IDLE, ST_END };

public:
	CGhost();
	virtual ~CGhost();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	STATE		GetCurState() { return m_eCurState; }
	void		SetCurState(STATE _curState) { m_eCurState = _curState; }

	void		SetHp(int _hp) { m_iHp += _hp; };

private:
	void		Follow();

	void		Motion_Change();

private:
	STATE		m_ePreState;
	STATE		m_eCurState;

	bool		m_bFirstAttack = true;
	bool		m_bFirstDamaged = true;

	float		m_fFirstX = WINCX + 50.f;
	float		m_fFirstY = -50.f;
private:
	DWORD				m_dwTime;
};
