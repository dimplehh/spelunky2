#pragma once

#include "Line.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	bool		Collision_Line(float& fX, float* pY);
	bool		Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);
	bool		Collision_Line(float& fX, float& fY, float& fCX, float& fCY);
	float		Equation_Line(float& fX, float _x1, float _y1, float _x2, float _y2);
	bool		LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY);
	bool		Ladder_Line(float& fX, float& fY, float& fCX, float& fCY);
	CLine*		Get_AttachedLine() { return m_AttachedLine; };
	float		GetY() { return m_fY; };

public:
	void		Initialize();
	void		Render(HDC hDC);
	void		Release();
	void		Load_Line();
	void		Change_idx();

public:
	static CLineMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CLineMgr;

		return m_pInstance;
	}
	static void		Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CLineMgr*		m_pInstance;
	list<CLine*>			m_LineList;
	CLine*					m_AttachedLine;
	float					m_fY;
	int						m_iIdx = 1;
};

