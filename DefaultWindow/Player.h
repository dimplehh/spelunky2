#pragma once

#include "Obj.h"

class CPlayer :	public CObj
{
public:								//±âÀý	//À§¸¦ º½			// ¾þµå¸²			//±â¾î°¨	//ÀÏ¾î¼¶	//¶³¾îÁü
	enum STATE { IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, KNEELSTAY, CRAWL, STANDUP,		FALLING,
					ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, HANGON, ST_END };
							//¶³¾îÁú »·										// ¸Å´Þ¸²
public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	void			Update_Rect();

private:
	void		Key_Input();
	void		Offset();
	void		Motion_Change();
	bool		Check_Move_End();

	bool		Die();
	void		CheckFall();
	void		Gravity();
	void		InLadder();

	void		HoldLeft();
	void		HoldRight();
	void		HoldUp();
	void		HoldDown();
	void		TapZ();

private:

	POINT		m_tPosin;
	float		m_fDistance;
	int			m_iHp;

	bool		m_bJump;
	bool		m_bLadder;
	bool		m_bKneelDown; //¾þµå¸° »óÅÂ
	int			m_iJumpCount;

	float		m_fPower;
	float		m_fTime;

	STATE		m_ePreState;
	STATE		m_eCurState;

	float		m_fPreY;
	float		m_fCurY;
	float		m_fDiffY;

	float		m_fMoveOffset = 0;
};





