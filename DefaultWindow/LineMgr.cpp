#include "stdafx.h"
#include "LineMgr.h"
#include "TileMgr.h"
#include <string>

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

bool CLineMgr::Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping) //�ٴ��浹 - ������������
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								// �浹 x

	m_AttachedLine = nullptr;

	if (!_Jumping) //���� ���°� �ƴ� ���� ������� �浹�����϶�� ��
	{
		for (auto& iter : m_LineList)
		{
			if (iter->Get_LineType() == CLine::FLOOR
				&& iter->Get_Info().tLPoint.fX <= fX + fCX / 6 && fX - fCX / 6 < iter->Get_Info().tRPoint.fX
				&& fY >= iter->Get_Info().tLPoint.fY - fCY / 2)
			{
				m_fY = iter->Get_Info().tLPoint.fY;										//�÷��̾� ��ġ�� ��������ִ� �κ�

				if (((fY + (fCY / 6.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))		// �ϴ� �κ� �浹 ���� ���� ��ü �������� 1/6 ����
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

bool CLineMgr::Collision_Line_Ceiling(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping)	//õ�� �浹
{
	if (m_LineList.empty())
		return 0;

	m_AttachedLine = nullptr;
	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX < fX && fX < iter->Get_Info().tRPoint.fX)
			&& (iter->Get_Info().tLPoint.fY - 10 < fY - fCY / 2 && fY - fCY / 2 < iter->Get_Info().tLPoint.fY + 10)
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

int		CLineMgr::Collision_Vertical_Line(float& fX, float& fY, float& fCX, float& fCY)	//���浹
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
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								// �ϴܿ� �� ����

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
			&& (iter->Get_LineType() == CLine::LEFTWALL || iter->Get_LineType() == CLine::RIGHTWALL))		//�� ���� ���ִ� ���
		{
			m_AttachedLine = iter;
			return true;
		}
	}
	if (!m_AttachedLine)
		return false;
}

bool CLineMgr::Can_Hang_Line(float fPointX, float fPointY, float& fX, float& fY, float& fCX, float& fCY, bool bJumping)	// �Ŵ޸� �� �ִ��� �Ǵ��ϴ� �ڵ�
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

//��Ÿ�� ���� �� �Ǵ� �Լ�
bool CLineMgr::Ladder_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								//  false
	for (auto& iter : m_LineList)
	{
		if (iter->Get_LineType() == CLine::LADDER)
		{
			if ((fX - (fCX / 2.f) < iter->Get_Info().tLPoint.fX) && (iter->Get_Info().tLPoint.fX < fX + (fCX / 2.f)))
			{
				if ((iter->Get_Info().tLPoint.fY >= fY - (fCY / 2.f)) && (fY - (fCY / 2.f) > iter->Get_Info().tRPoint.fY) ||
					(iter->Get_Info().tLPoint.fY >= fY + (fCY / 2.f)) && (fY + (fCY / 2.f) > iter->Get_Info().tRPoint.fY))
				{  //- �÷��̾� fy �� y1 ~y2 ���̿� �����ϸ� true
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

float CLineMgr::Equation_Line(float& fX, float _x1, float _y1, float _x2, float _y2)
{
	return ((_y2 - _y1) / (_x2 - _x1)) * (fX - _x1) + _y1;
}

void CLineMgr::SetLine()
{
	m_LineList.push_back(new CLine({ LINEPOINT{2 * TILECX, 1 * TILECY}, LINEPOINT{(TILEX - 2) * TILECX, 1 * TILECY} }));							// õ��
	m_LineList.back()->Set_LineType(CLine::CEILING);
	m_LineList.push_back(new CLine({ LINEPOINT{2 * TILECX, (TILEY - 2) * TILECY}, LINEPOINT{(TILEX - 2) * TILECX, (TILEY - 2) * TILECY} }));		// �ٴ�
	m_LineList.back()->Set_LineType(CLine::FLOOR);
	m_LineList.push_back(new CLine({ LINEPOINT{2 * TILECX, 1 * TILECY}, LINEPOINT{2 * TILECX, (TILEY - 2) * TILECY} }));							// ���� ��
	m_LineList.back()->Set_LineType(CLine::LEFTWALL);
	m_LineList.push_back(new CLine({ LINEPOINT{(TILEX - 2) * TILECX, 1 * TILECY}, LINEPOINT{(TILEX - 2) * TILECX, (TILEY - 2) * TILECY} }));	// ������ ��
	m_LineList.back()->Set_LineType(CLine::RIGHTWALL);

	m_LineList.push_back(new CLine({ LINEPOINT{22 * TILECX, 6 * TILECY}, LINEPOINT{22 * TILECX, 4 * TILECY} }));									// ��ٸ�
	m_LineList.back()->Set_LineType(CLine::LADDER);

	LINE	tInfo{};
	INFO	iterInfo{};

	if (nullptr != m_pVecTile)
	{
		for (int _index = 0; _index < m_pVecTile->size(); _index++)
		{
			int _y = _index / TILEX;
			int _x = _index % TILEX;
			iterInfo = (*m_pVecTile)[_index]->Get_Info();
			if (2 < _x && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - 1]->Get_Option() == 0) //���� ��
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2}, 
					LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} }));
				m_LineList.back()->Set_LineType(CLine::LEFTWALL);
			}
			if (_x < TILEX - 1 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + 1]->Get_Option() == 0) //������ ��
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} }));
				m_LineList.back()->Set_LineType(CLine::RIGHTWALL);
			}
			if (2 < _y && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - TILEX]->Get_Option() == 0) //��� ��
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2} }));
				m_LineList.back()->Set_LineType(CLine::FLOOR);
			}
			if (_y < TILEY - 2 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + TILEX]->Get_Option() == 0) // õ��
			{
				m_LineList.push_back(new CLine({ LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} }));
				m_LineList.back()->Set_LineType(CLine::CEILING);
			}
		}
	}
}

void CLineMgr::Change_idx()
{
	m_iIdx++;
	Release();
}