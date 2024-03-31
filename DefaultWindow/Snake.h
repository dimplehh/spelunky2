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

private:
	void		Motion_Change();

private:
	STATE		m_ePreState;
	STATE		m_eCurState;

private:
	DWORD				m_dwTime;
};
