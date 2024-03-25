#pragma once

#include "Obj.h"

class CPlayer :	public CObj
{
public:								//기절	//위를 봄			// 엎드림			//기어감	//일어섬	//떨어짐
	enum STATE { IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, KNEELSTAY, CRAWL, STANDUP,		FALLING,
					ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, HANGON, ST_END };
							//떨어질 뻔										// 매달림
public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void		SetAttachedBox(bool _isAttached) { m_bAttachedBox = _isAttached; }

private:
	void		SetRenderImage(HDC hDC);

	void		Key_Input();
	void		Offset();
	void		Motion_Change();
	bool		Check_Move_End();

	bool		Die();
	void		Gravity();
	void		FallDamage();
	void		AlmostFell();
	void		CanHanging();
	void		InLadder();

	void		HoldLeft();
	void		HoldRight();
	void		HoldUp();
	void		HoldDown();
	void		TapZ();
	

private:

	// POINT		m_tPosin; // 나중에 총 구현할때 사용할 수 있을듯
	float		m_fDistance;
	int			m_iHp;

	bool		m_bJump;
	bool		m_bLadder;
	bool		m_bCanHang;
	bool		m_bKneelDown; //엎드린 상태
	bool		m_bAttachedBox;	//상자와 닿은 상태
	int			m_iJumpCount;

	float		m_fPower;
	float		m_fTime;

	STATE		m_ePreState;
	STATE		m_eCurState;

	float		m_fPreY = 0;
	float		m_fCurY = 0;
	float		m_fDiffY = 0;

	float		m_fMoveOffset = 0;

private:
	DWORD				m_dwTime;
};





