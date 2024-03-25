#include "stdafx.h"
#include "LineMgr.h"
#include "TileMgr.h"
#include <string>

CLineMgr*	CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr(): m_fY(0)
{
	m_AttachedLine = nullptr;
	m_pVecTile = nullptr;
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();
	SetLine();
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), Safe_Delete<CLine*>);
	m_LineList.clear();
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& iter : m_LineList)
		iter->Render(hDC);
}

bool CLineMgr::Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 충돌 x

	m_AttachedLine = nullptr;

	if (!_Jumping)
	{
		for (auto& iter : m_LineList)
		{
			if (iter->Get_Info().tLPoint.fY == iter->Get_Info().tRPoint.fY
				&& (iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX)) //수평선이면
			{
				m_fY = iter->Get_Info().tLPoint.fY;

				if (((fY + (fCY / 6.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))
				{    // 하단 부분 충돌 범위 지정 전체 사이즈의 1/6 가량
					m_AttachedLine = iter;
					fY = m_fY - (fCY / 2);
					return true;
				}
			}
		}
	}
	if (!m_AttachedLine)
		return false;
}

bool CLineMgr::LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 하단에 선 없음

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX <= fX - (fCX / 2.f)) && (fX - (fCX / 2.f) < iter->Get_Info().tRPoint.fX) ||
			(iter->Get_Info().tLPoint.fX <= fX + (fCX / 2.f)) && (fX + (fCX / 2.f) < iter->Get_Info().tRPoint.fX))
		{
			//float x1 = iter->Get_Info().tLPoint.fX;
			//float y1 = iter->Get_Info().tLPoint.fY;
			//float x2 = iter->Get_Info().tRPoint.fX;
			//float y2 = iter->Get_Info().tRPoint.fY;
			//m_fY = Equation_Line(fX, x1, y1, x2, y2);
			//if (fY + (fCY / 2.f) < m_fY)
			//{
			//	return true;
			//}
		}
	}
	return false;
}

//줄타기 전용 선 판단 함수
bool CLineMgr::Ladder_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								//  false
	for (auto& iter : m_LineList)
	{
		if (iter->Get_Info().tLPoint.fX == iter->Get_Info().tRPoint.fX
			&& abs(iter->Get_Info().tLPoint.fY - iter->Get_Info().tRPoint.fY) > 50)
		{      //- 선 x1 == x2 이고
			if ((fX - (fCX / 2.f) < iter->Get_Info().tLPoint.fX) && (iter->Get_Info().tLPoint.fX < fX + (fCX / 2.f)))
			{
				if ((iter->Get_Info().tLPoint.fY >= fY - (fCY / 2.f)) && (fY - (fCY / 2.f) > iter->Get_Info().tRPoint.fY) ||
					(iter->Get_Info().tLPoint.fY >= fY + (fCY / 2.f)) && (fY + (fCY / 2.f) > iter->Get_Info().tRPoint.fY))
				{  //- 플레이어 fy 가 y1 ~y2 사이에 존재하면 true
					//fX = iter->Get_Info().tLPoint.fX;

					//float x1 = iter->Get_Info().tLPoint.fX;
					//float y1 = iter->Get_Info().tLPoint.fY;
					//float x2 = iter->Get_Info().tRPoint.fX;
					//float y2 = iter->Get_Info().tRPoint.fY;

					//m_fY = Equation_Line(fX, x1, y1, x2, y2);

					return true;
				}
			}
		}
	}
	return false;
}

bool CLineMgr::Can_Hang_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								//  false
	for (auto& iter : m_LineList)
	{
		if (iter->Get_Info().tLPoint.fX == iter->Get_Info().tRPoint.fX
			&& abs(iter->Get_Info().tLPoint.fY - iter->Get_Info().tRPoint.fY) <= 5)
		{      //- 선 x1 == x2 이고
			if ((fX - (fCX / 2.f) < iter->Get_Info().tLPoint.fX) && (iter->Get_Info().tLPoint.fX < fX + (fCX / 2.f)))
			{
				if ((iter->Get_Info().tLPoint.fY >= fY - (fCY / 2.f)) && (fY - (fCY / 2.f) > iter->Get_Info().tRPoint.fY) ||
					(iter->Get_Info().tLPoint.fY >= fY + (fCY / 2.f)) && (fY + (fCY / 2.f) > iter->Get_Info().tRPoint.fY))
				{
					//fX = iter->Get_Info().tLPoint.fX;

					//float x1 = iter->Get_Info().tLPoint.fX;
					//float y1 = iter->Get_Info().tLPoint.fY;
					//float x2 = iter->Get_Info().tRPoint.fX;
					//float y2 = iter->Get_Info().tRPoint.fY;

					//m_fY = Equation_Line(fX, x1, y1, x2, y2);

					return true;
				}
			}
		}
	}
	return false;
}

void CLineMgr::SetLine()
{
	m_LineList.push_back(new CLine({ LINEPOINT{2 * TILEX, TILECY * 1}, LINEPOINT{TILECX * (TILEX - 2), TILECY * 1} }));							// 천장
	m_LineList.push_back(new CLine({ LINEPOINT{2 * TILEX, TILECY * (TILEY - 2)}, LINEPOINT{TILECX * (TILEX - 2), TILECY * (TILEY - 2)} }));		// 바닥
	m_LineList.push_back(new CLine({ LINEPOINT{2 * TILEX, TILECY * 1}, LINEPOINT{2 * TILEX, TILECY * (TILEY - 2)} }));							// 왼쪽 벽
	m_LineList.push_back(new CLine({ LINEPOINT{TILECX * (TILEX - 2), TILECY * 1}, LINEPOINT{TILECX * (TILEX - 2), TILECY * (TILEY - 2)} }));	// 오른쪽 벽

	LINE	tInfo{};
	INFO	iterInfo{};

	if (nullptr != m_pVecTile)
	{
		for (int _index = 0; _index < m_pVecTile->size(); _index++)
		{
			int _y = _index / TILEX;
			int _x = _index % TILEX;
			iterInfo = (*m_pVecTile)[_index]->Get_Info();
			if (2 < _x && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - 1]->Get_Option() == 0) //왼쪽 벽
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2}, 
					LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} }));
			}
			if (_x < TILEX - 1 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + 1]->Get_Option() == 0) //오른쪽 벽
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} }));
			}
			if (2 < _y && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - TILEX]->Get_Option() == 0) //밟는 땅
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2} }));
			}
			if (_y < TILEY - 2 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + TILEX]->Get_Option() == 0) // 아래땅
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} }));
			}
		}
	}
}

void CLineMgr::Change_idx()
{
	m_iIdx++;
	Release();
}