#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CTileMgr*		CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = TILECX / 2.f + (TILECX * j);
			float fY = TILECY / 2.f + (TILECY * i);

			CObj*	pObj = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(dynamic_cast<CTile*>(pObj));
		}
	}
}

void CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)	//�ø�
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int	iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int	iMaxX = iCullX + WINCX / TILECX + 2;
	int	iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}

	SetDecoLand(hDC, iScrollX, iScrollY);
}

void CTileMgr::SetDecoLand(HDC hDC, int _iScrollX, int _iScrollY)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DecoLand");

	LINE	tInfo{};
	INFO	iterInfo{};

	for (int _index = 0; _index < m_vecTile.size(); _index++)
	{
		int _y = _index / TILEX;
		int _x = _index % TILEX;
		iterInfo = m_vecTile[_index]->Get_Info();

		if (!m_vecTile.empty())
		{
			if (2 < _x && m_vecTile[_index]->Get_DrawID() == 1 && m_vecTile[_index - 1]->Get_DrawID() == 0) //���� ��
			{
				GdiTransparentBlt(hDC, iterInfo.fX - iterInfo.fCX / 2 - 32 + _iScrollX, iterInfo.fY + _iScrollY - 32, 64, 64, hMemDC, 0, 0, 64, 64, RGB(58, 58, 58));
			}
			if (_x < TILEX - 1 && m_vecTile[_index]->Get_DrawID() == 1 && m_vecTile[_index + 1]->Get_DrawID() == 0) //������ ��
			{
				GdiTransparentBlt(hDC, iterInfo.fX + iterInfo.fCX / 2 - 32 + _iScrollX, iterInfo.fY + _iScrollY - 32, 64, 64, hMemDC, 64, 0, 64, 64, RGB(58, 58, 58));
			}
			if (1 < _y && m_vecTile[_index]->Get_DrawID() == 1 && m_vecTile[_index - TILEX]->Get_DrawID() == 0) //��� ��
			{
				GdiTransparentBlt(hDC, iterInfo.fX - 32 + _iScrollX, iterInfo.fY - iterInfo.fCY / 2 - 32 + _iScrollY, 64, 64, hMemDC, 128, 0, 64, 64, RGB(58, 58, 58));
			}
			if (_y < TILEY - 2 && m_vecTile[_index]->Get_DrawID() == 1 && m_vecTile[_index + TILEX]->Get_DrawID() == 0) // �Ʒ���
			{
				GdiTransparentBlt(hDC, iterInfo.fX - 32 + _iScrollX, iterInfo.fY + iterInfo.fCY / 2 - 32 + _iScrollY, 64, 64, hMemDC, 192, 0, 64, 64, RGB(58, 58, 58));
			}
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking_Tile(POINT ptMouse, int iDrawID, int iOption)
{
	int	x = ptMouse.x / TILECX;
	int	y = ptMouse.y / TILECY;

	int iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Option(iOption);
}

void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat",		// ���� ���(�̸��� ����)
		GENERIC_WRITE,			// ���� ���� ���(GENERIC_READ : �б� ����)
		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ������� �����ϰ��� �� �� ����� ���� ���� ����(NULL�� ��� �������� ����)
		NULL,					// ���� ���(NULL�� ��� �⺻ ���� ����)
		CREATE_ALWAYS,			// ���� ���(CREATE_ALWAYS : ������ ������ ����, ������ �����), (OPEN_EXISTING : ������ ���� ���� ����)
		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�(�ƹ��� Ư�� �Ӽ��� ���� ���� ����)
		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����(�츮�� ��� ���ϱ� ������ NULL)


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte(0);	// ���� ó�� ����
	int		iDrawID(0), iOption(0);

	for(auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(pTile)->Get_Option();

		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(pTile->Get_Info()), sizeof(INFO), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save �Ϸ�"), L"����", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat",		// ���� ���(�̸��� ����)
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
	int		iDrawID(0), iOption(0);
	INFO	tInfo{};

	Release();

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj*	pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		dynamic_cast<CTile*>(pObj)->Set_DrawID(iDrawID);
		dynamic_cast<CTile*>(pObj)->Set_Option(iOption);

		m_vecTile.push_back(dynamic_cast<CTile*>(pObj));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}





