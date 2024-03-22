#pragma once

#include "Obj.h"

class CPlayer :	public CObj
{
public:								//±âÀý	//À§¸¦ º½			// ¾þµå¸²				//±â¾î°¨	//ÀÏ¾î¼¶
	enum STATE { IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, KNEELSTAY, CRAWL, STANDUP, 
					ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, HANGON, ST_END };
							//¶³¾îÁú »·									// ¸Å´Þ¸²
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
	void		Offset();
	void		Motion_Change();

	void		Gravity();
	void		InLadder();

private:

	POINT		m_tPosin;
	float		m_fDistance;

	bool		m_bJump;
	bool		m_bLadder;
	bool		m_bKneelDown; //¾þµå¸° »óÅÂ

	float		m_fPower;
	float		m_fTime;

	STATE		m_ePreState;
	STATE		m_eCurState;
};





