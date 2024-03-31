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
	bool		Collision_Line(float& fX, float& fY, float fCX, float fCY);
	bool		Collision_Board_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);		// 상향, 하향점프 가능한 발판
	bool		Collision_Box_Line(float& fX, float& fY, float fCX, float fCY, bool _Jumping);		// 상자의 중력 적용용 함수
	int			Collision_Vertical_Line(float& fX, float& fY, float fCX, float fCY);					// 벽 ( 반환값으로 왼쪽벽, 오른쪽벽, 충돌  x 구분 )
	bool		Box_Collision_Vertical_Line(float& fX, float& fY, float& fCX, float& fCY);				// 상자 벽충돌

	bool		Collision_Line_Ceiling(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping);	// 천장	

	float		Equation_Line(float& fX, float _x1, float _y1, float _x2, float _y2);

	bool		LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY);
	bool		Ladder_Line(float& fX, float& fY, float& fCX, float& fCY);
	bool		Check_Almost_Fell(float& fX, float& fY, float& fCX, float& fCY);
	bool		Can_Hang_Line(float fPointX, float fPointY, float& fX, float& fY, float& fCX, float& fCY, bool bJumping);
	int			Check_Rope_Attach_Ceiling(float& fX, float& fY);
	void		MakeRopeLine(LINEPOINT _tLPoint, LINEPOINT _tRPoint);

	void		Set_Box_Line(float fX, float fY);
	list<CLine*>* Get_Box_Line() { return &m_BoxLineList; }

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
	list<CLine*>			m_BoxLineList;

	CLine*					m_AttachedLine;
	float					m_fX;
	float					m_fY;
	int						m_iIdx = 1;

	vector<CTile*>*			m_pVecTile;
};

