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
	//Load_Line();
	MyLine();
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

void CLineMgr::MyLine()
{
	LINE	tInfo{};
	INFO	iterInfo{};

	if (nullptr != m_pVecTile)
	{
		for (auto iter = m_pVecTile->begin(); iter != m_pVecTile->end(); ++iter)
		{
			iterInfo = dynamic_cast<CTile*>(*iter)->Get_Info();
			tInfo = { LINEPOINT{iterInfo.fX - iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2},
				LINEPOINT{iterInfo.fX + iterInfo.fCX / 2, iterInfo.fY + iterInfo.fCY / 2}};
			m_LineList.push_back(new CLine(tInfo));
		}
	}
	//LINE	tInfo{ LINEPOINT{100, 700}, LINEPOINT{800, 700} };
	//m_LineList.push_back(new CLine(tInfo)); 

	//tInfo = { LINEPOINT{300, 600}, LINEPOINT{600, 600} };
	//m_LineList.push_back(new CLine(tInfo));

	//tInfo = { LINEPOINT{300, 500}, LINEPOINT{600, 500} };
	//m_LineList.push_back(new CLine(tInfo));

	//tInfo = { LINEPOINT{700, 800}, LINEPOINT{700, 400} };
	//m_LineList.push_back(new CLine(tInfo));

	//tInfo = { LINEPOINT{800, 600}, LINEPOINT{800, 595} };
	//m_LineList.push_back(new CLine(tInfo));

	//tInfo = { LINEPOINT{128, 1536}, LINEPOINT{4032, 1536} };
	//m_LineList.push_back(new CLine(tInfo));
}

void CLineMgr::Load_Line()
{
	//	wstring fileStr = L"../Data/Line" + to_wstring(m_iIdx) + L".dat";
	HANDLE hFile = CreateFile(L"../Data/Line3.dat",	GENERIC_READ,NULL,NULL,OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte(0);	// 예외 처리 변수
	LINE	tInfo{};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINE), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		m_LineList.push_back(new CLine(tInfo));
	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CLineMgr::Change_idx()
{
	m_iIdx++;
	Release();
	Load_Line();
}

bool CLineMgr::Collision_Line(float& fX, float* pY)	// 이 아래것드 다 코드좀 깔끔하게 수정하자
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
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 충돌 x

	m_AttachedLine = nullptr;

	if (!_Jumping)
	{
		for (auto& iter : m_LineList)
		{
			if ((iter->Get_Info().tLPoint.fX <= fX - 10) && (fX - 10 < iter->Get_Info().tRPoint.fX) ||
				(iter->Get_Info().tLPoint.fX <= fX + 10) && (fX + 10 < iter->Get_Info().tRPoint.fX) ||
				((iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX)))
				// fx 좌 or 우 가 선 사이에 존재하면
			{
				float x1 = iter->Get_Info().tLPoint.fX;
				float y1 = iter->Get_Info().tLPoint.fY;
				float x2 = iter->Get_Info().tRPoint.fX;
				float y2 = iter->Get_Info().tRPoint.fY;

				m_fY = Equation_Line(fX, x1, y1, x2, y2);

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

bool CLineMgr::Collision_Line(float& fX, float& fY, float& fCX, float& fCY)
{
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 충돌 x

	m_AttachedLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if ((iter->Get_Info().tLPoint.fX <= fX) && (fX < iter->Get_Info().tRPoint.fX))
			// fx 좌 or 우 가 선 사이에 존재하면
		{
			float x1 = iter->Get_Info().tLPoint.fX;
			float y1 = iter->Get_Info().tLPoint.fY;
			float x2 = iter->Get_Info().tRPoint.fX;
			float y2 = iter->Get_Info().tRPoint.fY;

			m_fY = Equation_Line(fX, x1, y1, x2, y2);

			if (((fY + (fCY / 3.f)) <= m_fY) && (m_fY <= (fY + (fCY / 2.f))))
			{    // 하단 부분 충돌 범위 지정 전체 사이즈의 1/3 가량
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
	if (m_LineList.empty())							// 맵에 선이 없다면 
		return false;								// 하단에 선 없음

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
