#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	void			Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	void			Set_Dir(DIRECTION eDir)
	{
		m_eDir = eDir;
	}

	void		Set_PosY(float _fY) { m_tInfo.fY += _fY; }
	void		Set_PosX(float _fX) { m_tInfo.fX += _fX; }

	RENDERID		Get_GroupID() const { return m_eRender; }

	RECT			Get_Rect() { return m_tRect; }
	INFO			Get_Info() { return m_tInfo; }
	bool			Get_Dead() { return m_bDead; }
	void			Set_Dead() { m_bDead = true;  }
	void			Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void			Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	
	void			Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
public:
	virtual		void		Initialize()	PURE;
	virtual		int			Update()		PURE;
	virtual		void		Late_Update()	PURE;
	virtual		void		Render(HDC hDC)	PURE;
	virtual		void		Release()		PURE;

protected:
	void		Update_Rect();
	void		Move_Frame();
	void		Set_Frame(int _frameStart, int _frameEnd, int _motion, bool _roop, int _dwSpeed);
	void		Set_Frame(int _frameStart, int _frameEnd, int _motion, bool _roop, int _dwSpeed, int _repeat);
	void		Set_Frame(int _frameStart, int _frameEnd, int _motion, bool _roop, int _dwSpeed, int _repeat, int _frameMax);

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	int			m_iFirstFrameStart;
	int			m_iRepeatCount = 0;

	DIRECTION	m_eDir;
	RENDERID	m_eRender;

	float		m_fSpeed;
	bool		m_bDead;
	bool		m_bFlip;

	float		m_fAngle;

	CObj*		m_pTarget;
	const TCHAR*		m_pFrameKey = L"";

};

