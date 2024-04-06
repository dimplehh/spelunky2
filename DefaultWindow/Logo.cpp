#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

extern float g_fBgmVolume;

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Title.bmp", L"Logo");
	m_tFrame = { 0, 5, 0, 5, true, 80, GetTickCount(), 0 };
	m_pFrameKey = L"Logo";
	CSoundMgr::Get_Instance()->PlayBGM(L"Menu.wav", g_fBgmVolume);
}

int CLogo::Update()
{
	return 0;
}

void CLogo::Late_Update()
{

	Move_Frame();

	if (CKeyMgr::CreateSingleTonInst()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
		return;
	}
}


void CLogo::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, m_tFrame.iFrameStart * WINCX, 0, SRCCOPY);
}

void CLogo::Release()
{
}

void CLogo::Move_Frame()
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
