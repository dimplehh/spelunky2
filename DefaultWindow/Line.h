#pragma once

#include "Define.h"

class CLine
{
public:
	CLine();
	CLine(LINEPOINT& tLPoint, LINEPOINT& tRPoint);
	CLine(LINE& tLine);
	~CLine();

public:
	const LINE&		Get_Info() const { return m_tInfo; }

public:
	void		Render(HDC hDC);

private:
	LINE		m_tInfo;
};

