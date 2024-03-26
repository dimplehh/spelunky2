#include "stdafx.h"
#include "LineMgr.h"
#include "TileMgr.h"
#include <string>
#include "AbstractFactory.h"

CLineMgr*	CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr(): m_fX(0), m_fY(0)
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

bool CLineMgr::Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping) //바닥충돌 - 상향점프까지
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 충돌 x

	m_AttachedLine = nullptr;

	if (!_Jumping) //점프 상태가 아닐 때만 지면과의 충돌판정하라는 뜻
	{
		for (auto& iter : m_LineList)
		{
			if ((iter->Get_LineType() == CLine::FLOOR || iter->Get_LineType() == CLine::BOARD)
				&& iter->Get_Info().tLPoint.fX <= fX + fCX / 6 && fX - fCX / 6 < iter->Get_Info().tRPoint.fX
				&& fY >= iter->Get_Info().tLPoint.fY - fCY / 2)
			{
				m_fY = iter->Get_Info().tLPoint.fY;										//플레이어 위치를 변경시켜주는 부분

				if (((fY + (fCY / 6.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))		// 하단 부분 충돌 범위 지정 전체 사이즈의 1/6 가량
				{
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

bool CLineMgr::Collision_Board_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping)
{
	if (m_LineList.empty())
		return false;

	m_AttachedLine = nullptr;

	if (!_Jumping) //점프 상태가 아닐 때만 지면과의 충돌판정하라는 뜻
	{
		for (auto& iter : m_LineList)
		{
			if (iter->Get_LineType() == CLine::BOARD
				&& iter->Get_Info().tLPoint.fX <= fX + fCX / 6 && fX - fCX / 6 < iter->Get_Info().tRPoint.fX
				&& fY >= iter->Get_Info().tLPoint.fY - fCY / 2)
			{
				m_fY = iter->Get_Info().tLPoint.fY;										//플레이어 위치를 변경시켜주는 부분

				if (((fY + (fCY / 6.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))		// 하단 부분 충돌 범위 지정 전체 사이즈의 1/6 가량
				{
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

bool CLineMgr::Collision_Line_Ceiling(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping)	//천장 충돌
{
	if (m_LineList.empty())
		return 0;

	m_AttachedLine = nullptr;
	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX < fX && fX < iter->Get_Info().tRPoint.fX)
			&& (iter->Get_Info().tLPoint.fY - 10 < fY && fY - fCY / 2 < iter->Get_Info().tLPoint.fY + 10)
			&& iter->Get_LineType() == CLine::CEILING)
		{
			m_fY = iter->Get_Info().tLPoint.fY;
			m_AttachedLine = iter;
			fY = m_fY + fCY / 2;
			return true;
		}
	}
	if (!m_AttachedLine)
		return 0;
}

int		CLineMgr::Collision_Vertical_Line(float& fX, float& fY, float& fCX, float& fCY)	//벽충돌
{
	if (m_LineList.empty())
		return 0;

	m_AttachedLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX - 10 < fX && fX < iter->Get_Info().tLPoint.fX + 10)
			&& (iter->Get_Info().tLPoint.fY < fY && fY < iter->Get_Info().tRPoint.fY))
		{
			m_fX = iter->Get_Info().tLPoint.fX;

			if (iter->Get_LineType() == CLine::LEFTWALL)
			{
				m_AttachedLine = iter;
				fX = m_fX - 10;
				return -1;
			}
			else if (iter->Get_LineType() == CLine::RIGHTWALL)
			{
				m_AttachedLine = iter;
				fX = m_fX + 10;
				return 1;
			}
		}
	}
	if (!m_AttachedLine)
		return 0;
}


bool CLineMgr::LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 하단에 선 없음

	for (auto& iter : m_LineList)
	{
		if (iter->Get_Info().tLPoint.fY == iter->Get_Info().tRPoint.fY && (iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX)
			&& iter->Get_Info().tRPoint.fX - iter->Get_Info().tRPoint.fX > 1000)
		{
			return true;
		}
	}
	return false;
}

bool CLineMgr::Check_Almost_Fell(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())
		return false;

	m_AttachedLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX - 10 < fX && fX < iter->Get_Info().tLPoint.fX + 10)
			&& (fY < iter->Get_Info().tLPoint.fY && iter->Get_Info().tLPoint.fY - fCY - 10 < fY)
			&& (iter->Get_LineType() == CLine::LEFTWALL || iter->Get_LineType() == CLine::RIGHTWALL))		//벽 위에 서있는 경우
		{
			m_AttachedLine = iter;
			return true;
		}
	}
	if (!m_AttachedLine)
		return false;
}

bool CLineMgr::Can_Hang_Line(float fPointX, float fPointY, float& fX, float& fY, float& fCX, float& fCY, bool bJumping)	// 매달릴 수 있는지 판단하는 코드
{
	if (m_LineList.empty())
		return false;

	m_AttachedLine = nullptr;

	if (bJumping)
	{
		for (auto& iter : m_LineList)
		{
			if (((iter->Get_Info().tLPoint.fX == fPointX && iter->Get_Info().tLPoint.fY == fPointY) && fX < iter->Get_Info().tLPoint.fX) && iter->Get_LineType() == CLine::FLOOR)
			{
				fX = iter->Get_Info().tLPoint.fX - 15;
				fY = iter->Get_Info().tLPoint.fY + 10;
				m_AttachedLine = iter;
				return true;
			}
			else if (((iter->Get_Info().tRPoint.fX == fPointX && iter->Get_Info().tRPoint.fY == fPointY) && iter->Get_Info().tRPoint.fX < fX) && iter->Get_LineType() == CLine::FLOOR)
			{
				fX = iter->Get_Info().tRPoint.fX + 15;
				fY = iter->Get_Info().tRPoint.fY + 10;
				m_AttachedLine = iter;
				return true;
			}
		}
	}
	if (!m_AttachedLine)
		return false;
}

//줄타기 전용 선 판단 함수
bool CLineMgr::Ladder_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								//  false
	for (auto& iter : m_LineList)
	{
		if (iter->Get_LineType() == CLine::LADDER)
		{
			if ((fX - (fCX / 2.f) < iter->Get_Info().tLPoint.fX) && (iter->Get_Info().tLPoint.fX < fX + (fCX / 2.f)))
			{
				if ((iter->Get_Info().tLPoint.fY >= fY - (fCY / 2.f)) && (fY - (fCY / 2.f) >= iter->Get_Info().tRPoint.fY) ||
					(iter->Get_Info().tLPoint.fY >= fY + (fCY / 2.f)) && (fY + (fCY / 2.f) > iter->Get_Info().tRPoint.fY))
				{  
					fX = iter->Get_Info().tLPoint.fX;
					
					// 사다리 맨 위에 있을 경우 처리 마저 필요

					return true;
				}
			}
		}
	}
	return false;
}

float CLineMgr::Equation_Line(float& fX, float _x1, float _y1, float _x2, float _y2)
{
	return ((_y2 - _y1) / (_x2 - _x1)) * (fX - _x1) + _y1;
}

void CLineMgr::SetLine()
{
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
				m_LineList.push_back(CLineFactory::Create(	LINEPOINT{ iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2 }, LINEPOINT{ iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2 },CLine::LEFTWALL));
			if (_x < TILEX - 1 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + 1]->Get_Option() == 0) //오른쪽 벽
				m_LineList.push_back(CLineFactory::Create(	LINEPOINT{ iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2 }, LINEPOINT{ iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2 }, CLine::RIGHTWALL));
			if (0 < _y && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - TILEX]->Get_Option() == 0) //밟는 땅
				m_LineList.push_back(CLineFactory::Create(	LINEPOINT{ iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2 }, LINEPOINT{ iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2 }, CLine::FLOOR));
			if (_y < TILEY - 2 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + TILEX]->Get_Option() == 0) // 천장
				m_LineList.push_back(CLineFactory::Create(	LINEPOINT{ iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2 }, LINEPOINT{ iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2 }, CLine::CEILING));
		}
	}
	MakeEtcLine();	// 맵 상하좌우 끝, 사다리, 발판 함수
}

void CLineMgr::MakeEtcLine()
{
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 2 * TILECX, 1 * TILECY }, LINEPOINT{ (TILEX - 2) * TILECX, 1 * TILECY }, CLine::CEILING));							// 천장
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 2 * TILECX, (TILEY - 2) * TILECY }, LINEPOINT{ (TILEX - 2) * TILECX, (TILEY - 2) * TILECY }, CLine::FLOOR));		// 바닥
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 2 * TILECX, 1 * TILECY }, LINEPOINT{ 2 * TILECX, (TILEY - 2) * TILECY }, CLine::LEFTWALL));						// 왼쪽 벽
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ (TILEX - 2) * TILECX, 1 * TILECY }, LINEPOINT{ (TILEX - 2) * TILECX, (TILEY - 2) * TILECY }, CLine::RIGHTWALL));	// 오른쪽 벽

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ (17 + 0.5f) * TILECX, 18 * TILECY }, LINEPOINT{ (17 + 0.5f) * TILECX, 15 * TILECY }, CLine::LADDER));				// 사다리
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ (3 + 0.5f) * TILECX, 18 * TILECY }, LINEPOINT{ (3 + 0.5f) * TILECX, 14 * TILECY }, CLine::LADDER));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ (32 + 0.5f) * TILECX, 23 * TILECY }, LINEPOINT{ (32 + 0.5f) * TILECX, 19 * TILECY }, CLine::LADDER));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 4 * TILECX, 15 * TILECY + 20 }, LINEPOINT{ 5 * TILECX, 15 * TILECY + 10 }, CLine::BOARD));							// 발판
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 5 * TILECX, 15 * TILECY + 20 }, LINEPOINT{ 6 * TILECX, 15 * TILECY + 10 }, CLine::BOARD));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 15 * TILECX, 17 * TILECY + 15 }, LINEPOINT{ 16 * TILECX, 17 * TILECY + 15 }, CLine::BOARD));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 16 * TILECX, 17 * TILECY + 15 }, LINEPOINT{ 17 * TILECX, 17 * TILECY + 15 }, CLine::BOARD));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 19 * TILECX, 18 * TILECY + 15 }, LINEPOINT{ 20 * TILECX, 18 * TILECY + 15 }, CLine::BOARD));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 20 * TILECX, 18 * TILECY + 15 }, LINEPOINT{ 21 * TILECX, 18 * TILECY + 15 }, CLine::BOARD));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 25 * TILECX, 12 * TILECY + 25 }, LINEPOINT{ 26 * TILECX, 12 * TILECY + 25 }, CLine::BOARD));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 26 * TILECX, 12 * TILECY + 25 }, LINEPOINT{ 27 * TILECX, 12 * TILECY + 25 }, CLine::BOARD));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 27 * TILECX, 12 * TILECY + 25 }, LINEPOINT{ 28 * TILECX, 12 * TILECY + 25 }, CLine::BOARD));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 49 * TILECX, 22 * TILECY + 10 }, LINEPOINT{ 50 * TILECX, 22 * TILECY + 10 }, CLine::BOARD));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 54 * TILECX, 20 * TILECY + 10 }, LINEPOINT{ 55 * TILECX, 20 * TILECY + 10 }, CLine::BOARD));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 54 * TILECX, 21 * TILECY + 10 }, LINEPOINT{ 55 * TILECX, 21 * TILECY + 10 }, CLine::BOARD));
	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 54 * TILECX, 22 * TILECY + 10 }, LINEPOINT{ 55 * TILECX, 22 * TILECY + 10 }, CLine::BOARD));

	m_LineList.push_back(CLineFactory::Create(LINEPOINT{ 58 * TILECX, 21 * TILECY + 10 }, LINEPOINT{ 59 * TILECX, 21 * TILECY + 10 }, CLine::BOARD));
}

void CLineMgr::Change_idx()
{
	m_iIdx++;
	Release();
}