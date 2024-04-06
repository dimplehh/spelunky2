#include "stdafx.h"
#include "Ending.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "LineMgr.h"

extern float g_fBgmVolume;

CEnding::CEnding() : m_dwTime(GetTickCount())
{
}

CEnding::~CEnding()
{
	Release();
}

void CEnding::Initialize()
{
	__super::m_iMapNum = 5;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Ending.bmp", L"Ending");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Font/Font.bmp", L"Font");
	m_tFrame = { 0, 5, 0, 5, true, 200, GetTickCount(), 0 };
	m_pFrameKey = L"Ending";
	CSoundMgr::Get_Instance()->PlayBGM(L"Ending.wav", g_fBgmVolume);

	float _responPosX = 100;
	float _responPosY = 460;
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetRespawnPos(_responPosX, _responPosY);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(_responPosX, _responPosY);
	CLineMgr::Get_Instance()->Initialize();

	CScrollMgr::Get_Instance()->Set_ScrollXY(WINCX / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX,
		WINCY / 2 - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY);
}

int CEnding::Update()
{
	CObjMgr::Get_Instance()->Update();
	return 0;
}

void CEnding::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	//Move_Frame();
	#ifdef _DEBUG

	if (m_dwTime + 1000 < GetTickCount())
	{
		cout << CSceneMgr::Get_Instance()->GetTotalMoney() << "/" << CSceneMgr::Get_Instance()->GetTotalTime() << endl;
		m_dwTime = GetTickCount();
	}
#endif
}

void CEnding::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Ending");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, m_tFrame.iFrameStart * WINCX, 0, SRCCOPY); 
	
	CObjMgr::Get_Instance()->Render(hDC);
	SetNumberToFont(200, 200, hDC);
}

void CEnding::Release()
{
	CObjMgr::Get_Instance()->ReleaseWithoutPlayerAndUI();
}

void CEnding::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		m_tFrame.dwTime = GetTickCount();
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
		}
	}
}

void CEnding::SetNumberToFont(float _fX, float _fY, HDC  hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Font");

	int m_iNum = CSceneMgr::Get_Instance()->GetTotalMoney();
	int m_iSize = 16;

	if (m_iNum < 10)
		GdiTransparentBlt(hDC, _fX + 10, _fY, m_iSize, m_iSize, hMemDC, (m_iNum % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
	else
	{
		int iTemp = m_iNum;
		int addX = 0;
		while (1)
		{
			GdiTransparentBlt(hDC, _fX + 10 - 8.f * addX, _fY, m_iSize, m_iSize, hMemDC, (iTemp % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
			iTemp = iTemp / 10;
			addX++;
			if (iTemp == 0)
				break;
		}
	}
}

//void CEnding::SetTimeToFont(HDC hDC)
//{
//	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Font");
//
//	int _min = CUIMgr::Get_Instance()->Get_Time() / 60;
//	int _sec = CUIMgr::Get_Instance()->Get_Time() % 60;
//
//	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 0, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_min / 10) * 16, 0, 16, 16, RGB(63, 63, 63));
//	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 1, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_min % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
//	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 2, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, 10 * 16, 0, 16, 16, RGB(63, 63, 63));
//	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 3, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_sec / 10) * 16, 0, 16, 16, RGB(63, 63, 63));
//	GdiTransparentBlt(hDC, m_tInfo.fX + 10 + m_fTextX + 10.f * 4, m_tInfo.fY + m_fTextY, m_iSize, m_iSize, hMemDC, (_sec % 10) * 16, 0, 16, 16, RGB(63, 63, 63));
//}
