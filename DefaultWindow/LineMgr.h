#pragma once

#include "Line.h"

class CTile;

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	bool		Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);
	bool		Collision_Horizon_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);
	bool		Collision_Vertical_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);

	bool		LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY);	// 이제 이거 필요없을듯
	bool		Ladder_Line(float& fX, float& fY, float& fCX, float& fCY);
	bool		Can_Hang_Line(float& fX, float& fY, float& fCX, float& fCY);

	CLine*		Get_AttachedLine() { return m_AttachedLine; };
	float		Get_fY() { return m_fY; }

public:
	void		Initialize();
	void		Render(HDC hDC);
	void		Release();
	void		SetLine();
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
	vector<CTile*>*			m_pVecTile;
};

