#pragma once
#include "Monster.h"

class CSnake : public CMonster
{
public:
	enum STATE { IDLE, ATTACK, ST_END };

public:
	CSnake();
	virtual ~CSnake();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	STATE		GetCurState() { return m_eCurState; }
	void		SetCurState(STATE _curState) { m_eCurState = _curState; }

private:
	void		Motion_Change();
	void		Idle();
	void		Attack();

private:
	STATE		m_ePreState;
	STATE		m_eCurState;

	bool		m_bFirstAttack = true;

private:
	DWORD				m_dwTime;
};
