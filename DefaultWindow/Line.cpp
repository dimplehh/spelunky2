#include "stdafx.h"
#include "Line.h"
#include "ScrollMgr.h"


CLine::CLine()
{
	ZeroMemory(&m_tInfo, sizeof(LINE));
}

CLine::CLine(LINEPOINT & tLPoint, LINEPOINT & tRPoint) : m_tInfo(tLPoint, tRPoint)
{
}

CLine::CLine(LINE & tLine) : m_tInfo(tLine)
{
}

CLine::~CLine()
{
}

void CLine::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	MoveToEx(hDC, int(m_tInfo.tLPoint.fX + fScrollX), (int)m_tInfo.tLPoint.fY, nullptr);
	LineTo(hDC, int(m_tInfo.tRPoint.fX + fScrollX), (int)m_tInfo.tRPoint.fY);
}
