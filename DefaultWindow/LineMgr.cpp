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
			if (2 < _x && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - 1]->Get_Option() == 0) //���� ��
			{
				tInfo = { LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} };
				m_LineList.push_back(new CLine(tInfo));
			}
			if (_x < TILEX - 1 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + 1]->Get_Option() == 0) //������ ��
			{
				tInfo = { LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} };
				m_LineList.push_back(new CLine(tInfo));
			}
			if (2 < _y && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index - TILEX]->Get_Option() == 0) //��� ��
			{
				tInfo = { LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY - iterInfo.fCY / 2} };
				m_LineList.push_back(new CLine(tInfo));
			}
			if (_y < TILEY - 2 && (*m_pVecTile)[_index]->Get_Option() >= 1 && (*m_pVecTile)[_index + TILEX]->Get_Option() == 0) // �Ʒ���
			{
				tInfo = { LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2},
					LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2} };
				m_LineList.push_back(new CLine(tInfo));
			}
		}
	}
}

void CLineMgr::Change_idx()
{
	m_iIdx++;
	Release();
}

bool CLineMgr::Collision_Line(float& fX, float* pY)	// �� �Ʒ��͵� �� �ڵ��� ����ϰ� ��������
{
	if (m_LineList.empty())
		return false;

	m_AttachedLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_Info().tLPoint.fX && fX < iter->Get_Info().tRPoint.fX)
		{
			m_AttachedLine = iter;
		}
	}

	if (!m_AttachedLine)
		return false;

	float x1 = m_AttachedLine->Get_Info().tLPoint.fX;
	float y1 = m_AttachedLine->Get_Info().tLPoint.fY;
	float x2 = m_AttachedLine->Get_Info().tRPoint.fX;
	float y2 = m_AttachedLine->Get_Info().tRPoint.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

bool CLineMgr::Collision_Line(float& fX, float& fY, float& fCX, float& fCY, bool _Jumping)
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								// �浹 x

	m_AttachedLine = nullptr;

	if (!_Jumping)
	{
		for (auto& iter : m_LineList)
		{
			if ((iter->Get_Info().tLPoint.fX <= fX - 10) && (fX - 10 < iter->Get_Info().tRPoint.fX) ||
				(iter->Get_Info().tLPoint.fX <= fX + 10) && (fX + 10 < iter->Get_Info().tRPoint.fX) ||
				((iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX)))
				// fx �� or �� �� �� ���̿� �����ϸ�
			{
				float x1 = iter->Get_Info().tLPoint.fX;
				float y1 = iter->Get_Info().tLPoint.fY;
				float x2 = iter->Get_Info().tRPoint.fX;
				float y2 = iter->Get_Info().tRPoint.fY;

				m_fY = Equation_Line(fX, x1, y1, x2, y2);

				if (((fY + (fCY / 6.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))
				{    // �ϴ� �κ� �浹 ���� ���� ��ü �������� 1/6 ����
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

bool CLineMgr::Collision_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								// �浹 x

	m_AttachedLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX))
			// fx �� or �� �� �� ���̿� �����ϸ�
		{
			float x1 = iter->Get_Info().tLPoint.fX;
			float y1 = iter->Get_Info().tLPoint.fY;
			float x2 = iter->Get_Info().tRPoint.fX;
			float y2 = iter->Get_Info().tRPoint.fY;

			m_fY = Equation_Line(fX, x1, y1, x2, y2);

			if (((fY + (fCY / 3.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))
			{    // �ϴ� �κ� �浹 ���� ���� ��ü �������� 1/3 ����
				m_AttachedLine = iter;
				fY = m_fY - (fCY / 2.f);
				return true;
			}
		}
	}

	if (!m_AttachedLine)
		return false;
}

float CLineMgr::Equation_Line(float& fX, float _x1, float _y1, float _x2, float _y2)
{
	return ((_y2 - _y1) / (_x2 - _x1)) * (fX - _x1) + _y1;
}

bool CLineMgr::LastBottom_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								// �ϴܿ� �� ����

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX <= fX - (fCX / 2.f)) && (fX - (fCX / 2.f) < iter->Get_Info().tRPoint.fX) ||
			(iter->Get_Info().tLPoint.fX <= fX + (fCX / 2.f)) && (fX + (fCX / 2.f) < iter->Get_Info().tRPoint.fX))
		{
			float x1 = iter->Get_Info().tLPoint.fX;
			float y1 = iter->Get_Info().tLPoint.fY;
			float x2 = iter->Get_Info().tRPoint.fX;
			float y2 = iter->Get_Info().tRPoint.fY;
			m_fY = Equation_Line(fX, x1, y1, x2, y2);
			if (fY + (fCY / 2.f) < m_fY)
			{
				return true;
			}
		}
	}
	return false;
}

//��Ÿ�� ���� �� �Ǵ� �Լ�
bool CLineMgr::Ladder_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								//  false
	for (auto& iter : m_LineList)
	{
		if (iter->Get_Info().tLPoint.fX == iter->Get_Info().tRPoint.fX
			&& abs(iter->Get_Info().tLPoint.fY - iter->Get_Info().tRPoint.fY) > 50)
		{      //- �� x1 == x2 �̰�
			if ((fX - (fCX / 2.f) < iter->Get_Info().tLPoint.fX) && (iter->Get_Info().tLPoint.fX < fX + (fCX / 2.f)))
			{
				if ((iter->Get_Info().tLPoint.fY >= fY - (fCY / 2.f)) && (fY - (fCY / 2.f) > iter->Get_Info().tRPoint.fY) ||
					(iter->Get_Info().tLPoint.fY >= fY + (fCY / 2.f)) && (fY + (fCY / 2.f) > iter->Get_Info().tRPoint.fY))
				{  //- �÷��̾� fy �� y1 ~y2 ���̿� �����ϸ� true
					fX = iter->Get_Info().tLPoint.fX;

					float x1 = iter->Get_Info().tLPoint.fX;
					float y1 = iter->Get_Info().tLPoint.fY;
					float x2 = iter->Get_Info().tRPoint.fX;
					float y2 = iter->Get_Info().tRPoint.fY;

					m_fY = Equation_Line(fX, x1, y1, x2, y2);

					return true;
				}
			}
		}
	}
	return false;
}

bool CLineMgr::Can_Hang_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// �ʿ� ���� ���ٸ� 
		return false;								//  false
	for (auto& iter : m_LineList)
	{
		if (iter->Get_Info().tLPoint.fX == iter->Get_Info().tRPoint.fX
			&& abs(iter->Get_Info().tLPoint.fY - iter->Get_Info().tRPoint.fY) <= 5)
		{      //- �� x1 == x2 �̰�
			if ((fX - (fCX / 2.f) < iter->Get_Info().tLPoint.fX) && (iter->Get_Info().tLPoint.fX < fX + (fCX / 2.f)))
			{
				if ((iter->Get_Info().tLPoint.fY >= fY - (fCY / 2.f)) && (fY - (fCY / 2.f) > iter->Get_Info().tRPoint.fY) ||
					(iter->Get_Info().tLPoint.fY >= fY + (fCY / 2.f)) && (fY + (fCY / 2.f) > iter->Get_Info().tRPoint.fY))
				{
					fX = iter->Get_Info().tLPoint.fX;

					float x1 = iter->Get_Info().tLPoint.fX;
					float y1 = iter->Get_Info().tLPoint.fY;
					float x2 = iter->Get_Info().tRPoint.fX;
					float y2 = iter->Get_Info().tRPoint.fY;

					m_fY = Equation_Line(fX, x1, y1, x2, y2);

					return true;
				}
			}
		}
	}
	return false;
}
