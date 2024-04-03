#pragma once
#include "Monster.h"
#include "Tile.h"
#include "Line.h"

class COlmec : public CMonster
{
public:
	enum STATE { IDLE, RISE, SMASH, ST_END };

public:
	COlmec();
	virtual ~COlmec();

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
	bool		GetCanSmash() { return m_bCanSmash; }

public:
	void	SetHeadLine(CLine* headLine) { m_headLine = headLine; };

private:
	void		Phase0();
	void		Phase1();
	void		Phase2();

private:										// Phase 1
	void		Idle();
	void		Rise();
	void		Smash();

	void		Break();
	void		SetBrokenTile(int index);
	bool		GetUpNoBrokenTile(int index);

private:
	int			m_iAttackCycle = 3;
	float		m_fPreX = 0;
	float		m_fPrePlayerX = 0;
	float		m_fRealSmashX = 0;
	float		m_fPreY = 0;

	bool		m_bCanRise = false;
	bool		m_bCanSmash = false;
	bool		m_bCheckOneTime = true;


private:										// Phase 2
	void		Broken();
	void		Idle2();
	void		Rise2();
	void		Move();
	void		Attack();
	void		Motion_Change();

private:
	float		m_fPreY2 = 0;

	bool		m_bCanRise2 = false;
	bool		m_bCanMove = false;

	STATE		m_ePreState;
	STATE		m_eCurState;

private:
	int			m_iPhase = 0;
	DWORD		m_dwTime;

	vector<CTile*>* m_pVecTile;
	CLine*		m_headLine;
};
