#pragma once

#include "Line.h"

class CTile;

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	bool		Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);			// 밟는 땅
	bool		Collision_Board_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);		// 상향, 하향점프 가능한 발판
	int			Collision_Vertical_Line(float& fX, float& fY, float& fCX, float& fCY);					// 벽 ( 반환값으로 왼쪽벽, 오른쪽벽, 충돌  x 구분 )
	bool		Collision_Line_Ceiling(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);	// 천장	

	float		Equation_Line(float& fX, float _x1, float _y1, float _x2, float _y2);

	bool		LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY);
	bool		Ladder_Line(float& fX, float& fY, float& fCX, float& fCY);
	bool		Check_Almost_Fell(float& fX, float& fY, float& fCX, float& fCY);
	bool		Can_Hang_Line(float fPointX, float fPointY, float& fX, float& fY, float& fCX, float& fCY, bool bJumping);

	CLine*		Get_AttachedLine() { return m_AttachedLine; };
	float		Get_fY() { return m_fY; }

public:
	void		Initialize();
	void		Render(HDC hDC);
	void		Release();
	void		SetLine();
	void		MakeEtcLine();
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
	float					m_fX;
	float					m_fY;
	int						m_iIdx = 1;
	vector<CTile*>*			m_pVecTile;
};

