#pragma once
#include "Monster.h"

class CFrog : public CMonster
{
public:
	enum STATE { IDLE, JUMP, ST_END };

public:
	CFrog();
	virtual ~CFrog();

public:
	virtual void Initialize()		override;
	virtual int	 Update()			override;
	virtual void Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;

public:
	STATE		GetCurState() { return m_eCurState; }
	void		SetCurState(STATE _curState) { m_eCurState = _curState; }

	void		SetHp(int _hp) { m_iHp += _hp; };

private:
	void		Motion_Change();
	void		Idle();
	void		Jump();
	void		Attack();
	void		Damaged();

private:
	STATE		m_ePreState;
	STATE		m_eCurState;

	bool		m_bFirstAttack		= true;
	bool		m_bFirstDamaged		= true;
	bool		m_bFirstGrounded	= true;
	
	bool		m_bCanJump			= false;
	
	int			m_iJumpCycle		= 2;
	int			m_iFirstGroundTime	= 0;
	int			m_iMoveX			= 0;
	float		m_fPreY				= 0;

private:
	DWORD		m_dwTime;
};
