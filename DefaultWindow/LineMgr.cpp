#include "stdafx.h"
#include "LineMgr.h"
#include <string>

CLineMgr*	CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr(): m_fY(0)
{
	m_AttachedLine = nullptr;
}

CLineMgr::~CLineMgr()
{
	Release();
}

bool CLineMgr::Collision_Line(float & fX, float * pY)
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
			if ((iter->Get_Info().tLPoint.fX <= fX + (fCX / 2.f)) && (fX + (fCX / 2.f) < iter->Get_Info().tRPoint.fX) ||
				(iter->Get_Info().tLPoint.fX <= fX - (fCX / 2.f)) && (fX - (fCX / 2.f) < iter->Get_Info().tRPoint.fX) ||
				((iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX)))
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
		return false;								// ��Ÿ�� false

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX - iter->Get_Info().tRPoint.fX) < 5.f)
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


void CLineMgr::Initialize()
{
	Load_Line();
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& iter : m_LineList)
		iter->Render(hDC);
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), Safe_Delete<CLine*>);
	m_LineList.clear();
}

void CLineMgr::Load_Line()
{
	//	wstring fileStr = L"../Data/Line" + to_wstring(m_iIdx) + L".dat";
	HANDLE hFile = CreateFile(L"../Data/Line3.dat",		// ���� ���(�̸��� ����)
		GENERIC_READ,			// ���� ���� ���(GENERIC_READ : �б� ����)
		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ������� �����ϰ��� �� �� ����� ���� ���� ����(NULL�� ��� �������� ����)
		NULL,					// ���� ���(NULL�� ��� �⺻ ���� ����)
		OPEN_EXISTING,			// ���� ���(CREATE_ALWAYS : ������ ������ ����, ������ �����), (OPEN_EXISTING : ������ ���� ���� ����)
		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�(�ƹ��� Ư�� �Ӽ��� ���� ���� ����)
		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����(�츮�� ��� ���ϱ� ������ NULL)


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte(0);	// ���� ó�� ����
	LINE	tInfo{};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINE), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_LineList.push_back(new CLine(tInfo));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

void CLineMgr::Change_idx()
{
	m_iIdx++;
	Release();
	Load_Line();
}
