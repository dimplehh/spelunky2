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

CEnding::CEnding()
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
	//if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
	//{
	//	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	//	CSceneMgr::Get_Instance()->Scene_Change(SC_LOGO);
	//	return;
	//}
}

void CEnding::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Ending");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, m_tFrame.iFrameStart * WINCX, 0, SRCCOPY); 
	
	CObjMgr::Get_Instance()->Render(hDC);
	CLineMgr::Get_Instance()->Render(hDC);
}

void CEnding::Release()
{
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
