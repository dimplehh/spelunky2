#pragma once

#include "Define.h"

class CLine
{
public:
	enum LINETYPE {LEFTWALL,RIGHTWALL,CEILING,FLOOR,END};
public:
	CLine();
	CLine(LINEPOINT& tLPoint, LINEPOINT& tRPoint);
	CLine(LINE& tLine);
	~CLine();

public:
	const LINE&		Get_Info() const { return m_tInfo; }
	LINETYPE		Get_LineType() { return m_tLineType; }
	void			Set_LineType(LINETYPE _lineType) { m_tLineType = _lineType; }

public:
	void		Render(HDC hDC);

private:
	LINE		m_tInfo;
	LINETYPE	m_tLineType = FLOOR;
};

