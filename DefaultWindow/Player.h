#pragma once

#include "Obj.h"
#include "UIMgr.h"

class CPlayer :	public CObj
{
public:	
	enum STATE {IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, KNEELSTAY, CRAWL, STANDUP,	FALLING,
				HOLD, ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, HANGON, THROW, ST_END };
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
	bool		GetAttatchedBox() { return m_bAttachedBox; }
	STATE		GetState() { return m_eCurState; }
	void		SetState(STATE _state) { m_eCurState = _state; }
	bool		GetFlip() { return m_bFlip; }
	bool		GetKneelDown() { return m_bKneelDown; }
	bool		GetRevival() { return m_bRevival; }
	void		SetRevival(bool _revival) { m_bRevival = _revival; }
	int			GetDeathTime() { return m_iDeathTime; }
	int			GetHp() { return m_iHp; }
	void		SetHp(int _num);
	int			GetRopeCount() { return m_iRopeCount; };
	void		SetRopeCount() { m_iRopeCount = 0; CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_ROPE, m_iRopeCount); }
	bool		SetRopeCount(int _num)
	{
		if (m_iRopeCount + _num < 0)
			return false;
		else
		{
			m_iRopeCount += _num;
			CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_ROPE, m_iRopeCount);
			return true;
		}
	}

	int			GetBombCount() { return m_iBombCount; };
	void		SetBombCount() { m_iBombCount = 0; CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_BOMB, m_iBombCount); }
	bool		SetBombCount(int _num)
	{
		if (m_iBombCount + _num < 0)
			return false;
		else
		{
			m_iBombCount += _num;
			CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_BOMB, m_iBombCount);
			return true;
		}
	}

	int			GetMoney() { return m_iMoney; }
	void		SetMoney() { m_iMoney = 0; CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_MONEY, m_iMoney); }
	bool		SetMoney(int _num)
	{
		if (m_iMoney + _num < 0)
			return false;
		else
		{
			m_iMoney += _num;
			CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_MONEY, m_iMoney);
			return true;
		}
	}
	bool		GetCanHold() { return m_bCanHold; }
	void		SetCanHold(bool _canHold) { m_bCanHold = _canHold; }
	void		SetIsHold(bool _isHold) { m_bIsHold = _isHold; }
	bool		GetThrow() { return m_bThrow; }
	void		SetThrow(bool _throw) { m_bThrow = _throw; }

private:
	void		SetRenderImage(HDC hDC);
	void		ResetNum() 
	{
		SetRopeCount();
		SetBombCount();
		SetMoney();
	}

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
	void		TapX();
	void		TapZ();
	void		TapC();
	void		TapD();
	

private:

	// POINT		m_tPosin; // 나중에 총 구현할때 사용할 수 있을듯
	float		m_fFirstX;
	float		m_fFirstY;

	float		m_fDistance;
	int			m_iHp;

	bool		m_bJump;
	bool		m_bLadder;
	bool		m_bCanHang;
	bool		m_bKneelDown;	// 엎드린 상태
	bool		m_bAttachedBox;	// 상자와 닿은 상태
	bool		m_bCanHold;		// 잡을 수 있는 상태 ( 아래키 + x)
	bool		m_bIsHold;		// 무엇인가를 잡은 상태
	bool		m_bThrow;

	int			m_iJumpCount;

	float		m_fPower;
	float		m_fTime;

	STATE		m_ePreState;
	STATE		m_eCurState;

	float		m_fPreY = 0;
	float		m_fCurY = 0;
	float		m_fDiffY = 0;

	float		m_fMoveOffset = 0;

	bool		m_bAlmostFell = false;
	bool		m_bWallAttatched = false;

	bool		m_bRevival = false;

	bool		m_bFirstDieCheck = true;
	int			m_iDeathTime = 0;

	int			m_iRopeCount = 0;
	int			m_iBombCount = 0;
	int			m_iMoney = 0;

private:
	DWORD				m_dwTime;
};





