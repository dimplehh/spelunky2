#include "stdafx.h"
#include "LineMgr.h"

CLineMgr*	CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Release();
}

bool CLineMgr::Collision_Line(float & fX, float * pY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pTargetLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_Info().tLPoint.fX &&
			fX < iter->Get_Info().tRPoint.fX)
		{
			pTargetLine = iter;
		}
	}

	if (!pTargetLine)
		return false;

	//Y - y1 = ((y2 - y1) / (x2 - x1)) * (X - x1)
	// Y = ((y2 - y1) / (x2 - x1)) * (X - x1) + y1

	float x1 = pTargetLine->Get_Info().tLPoint.fX;
	float y1 = pTargetLine->Get_Info().tLPoint.fY;
	float x2 = pTargetLine->Get_Info().tRPoint.fX;
	float y2 = pTargetLine->Get_Info().tRPoint.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
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
	HANDLE hFile = CreateFile(L"../Data/Line.dat",		// ���� ���(�̸��� ����)
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
