#pragma once

#include "Obj.h"

class CPlayer :	public CObj
{
public:
	enum STATE { IDLE, WALK, ATTACK, HIT, DEAD, ST_END };

public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void		Key_Input();
	void		Jump();
	void		Offset();
	void		Motion_Change();

private:

	POINT				m_tPosin;
	float				m_fDistance;

	bool				m_bJump;


	float				m_fPower;
	float				m_fTime;

	STATE				m_ePreState;
	STATE				m_eCurState;


	// y = 파워(힘) * sin(90) * 시간 - (9.8f * 시간 * 시간) / 2

};





