#pragma once

#include "Obj.h"
#include "UIMgr.h"

class CPlayer :	public CObj
{
public:								//기절	//위를 봄			// 엎드림			//기어감	//일어섬	//떨어짐
	enum STATE { IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, KNEELSTAY, CRAWL, STANDUP,		FALLING,
					ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, HANGON, THROW, ST_END };
							//떨어질 뻔										// 매달림//던지기
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
	bool		GetFlip() { return m_bFlip; }
	bool		GetKneelDown() { return m_bKneelDown; }
	int			GetHp() { return m_iHp; }
	void		SetHp(int _num)
	{
		if (m_iHp + _num <= 0)
			m_iHp = 0;
		else
			m_iHp += _num;
		CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_HP, m_iHp);
	}
	bool		SetRopeCount(int _num)
	{
		if (m_iRopeCount + _num < 0)
		{
			return false;
		}
		else
		{
			m_iRopeCount += _num;
			CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_ROPE, m_iRopeCount);
			return true;
		}
	}
	bool		SetBombCount(int _num)
	{
		if (m_iBombCount + _num < 0)
		{
			return false;
		}
		else
		{
			m_iBombCount += _num;
			CUIMgr::Get_Instance()->Set_UINum(CUIIcon::UI_BOMB, m_iBombCount);
			return true;
		}
	}
	int			GetRopeCount() { return m_iRopeCount; };
	int			GetBombCount() { return m_iBombCount; };

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

	bool		m_bAlmostFell = false;
	bool		m_bWallAttatched = false;

	int			m_iRopeCount = 3;
	int			m_iBombCount = 3;

private:
	DWORD				m_dwTime;
};





