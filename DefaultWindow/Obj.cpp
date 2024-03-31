#include "stdafx.h"
#include "Obj.h"


CObj::CObj() : m_fSpeed(0.f), m_eDir(DIR_END),m_bFlip(false), m_iRepeatCount(0),
m_bDead(false), m_fAngle(0.f), m_eRender(RENDER_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= long(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= long(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= long(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= long(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Set_Frame(int _frameStart, int _frameEnd, int _motion, bool _roop, int _dwSpeed)
{
	if (m_bFlip == false)
	{
		m_tFrame.iFrameStart = _frameStart;
		m_tFrame.iFrameEnd = _frameEnd;
	}
	else
	{
		m_tFrame.iFrameStart = m_tFrame.iFrameMax - _frameStart;
		m_tFrame.iFrameEnd = m_tFrame.iFrameMax - _frameEnd;
	}
	m_iFirstFrameStart = m_tFrame.iFrameStart;

	m_tFrame.iMotion = _motion;
	m_tFrame.bRoop = _roop;
	m_tFrame.dwSpeed = _dwSpeed;
	m_tFrame.dwTime = GetTickCount();
}

void CObj::Set_Frame(int _frameStart, int _frameEnd, int _motion, bool _roop, int _dwSpeed, int _repeat)
{
	if (m_bFlip == false)
	{
		m_tFrame.iFrameStart = _frameStart;
		m_tFrame.iFrameEnd = _frameEnd;
	}
	else
	{
		m_tFrame.iFrameStart = m_tFrame.iFrameMax - _frameStart;
		m_tFrame.iFrameEnd = m_tFrame.iFrameMax - _frameEnd;
	}
	m_iFirstFrameStart = m_tFrame.iFrameStart;

	m_tFrame.iMotion = _motion;
	m_tFrame.bRoop = _roop;
	m_tFrame.dwSpeed = _dwSpeed;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.iRepeat = _repeat;
}

void CObj::Set_Frame(int _frameStart, int _frameEnd, int _motion, bool _roop, int _dwSpeed, int _repeat, int _frameMax)
{
	m_tFrame.iFrameMax = _frameMax;
	if (m_bFlip == false)
	{
		m_tFrame.iFrameStart = _frameStart;
		m_tFrame.iFrameEnd = _frameEnd;
	}
	else
	{
		m_tFrame.iFrameStart = m_tFrame.iFrameMax - _frameStart;
		m_tFrame.iFrameEnd = m_tFrame.iFrameMax - _frameEnd;
	}
	m_iFirstFrameStart = m_tFrame.iFrameStart;

	m_tFrame.iMotion = _motion;
	m_tFrame.bRoop = _roop;
	m_tFrame.dwSpeed = _dwSpeed;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.iRepeat = _repeat;
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		m_tFrame.dwTime = GetTickCount();

		if (m_bFlip == false)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
			{
				if (m_tFrame.bRoop == false)
				{
					if (m_iRepeatCount >= m_tFrame.iRepeat)
					{
						m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
						m_iRepeatCount = m_tFrame.iRepeat;
					}
					else
					{
						m_tFrame.iFrameStart = m_iFirstFrameStart;
						m_iRepeatCount++;
					}
				}
				else if (m_tFrame.bRoop == true)
				{
					m_tFrame.iFrameStart = m_iFirstFrameStart;
				}
			}
		}
		else
		{
			--m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart <= m_tFrame.iFrameEnd)
			{
				if (m_tFrame.bRoop == false)
				{
					if (m_iRepeatCount >= m_tFrame.iRepeat)
					{
						m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
						m_iRepeatCount = m_tFrame.iRepeat;
					}
					else
					{
						m_tFrame.iFrameStart = m_iFirstFrameStart;
						m_iRepeatCount++;
					}
				}
				else if (m_tFrame.bRoop == true)
				{
					m_tFrame.iFrameStart = m_iFirstFrameStart;
				}
			}
		}
	}
}
