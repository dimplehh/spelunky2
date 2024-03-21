#include "stdafx.h"
#include "MainGame.h"
#include "Monster.h"
#include "AbstractFactory.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"

CMainGame::CMainGame() : m_iFPS(0), m_dwTime(GetTickCount())
{

	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");

	CSceneMgr::Get_Instance()->Scene_Change(SC_LOGO);

}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();	
}

void CMainGame::Late_Update()
{
	CScrollMgr::Get_Instance()->Scroll_Lock();

	CSceneMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render()
{
	// FPS Ãâ·Â

	++m_iFPS;

	if(m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
	
	HDC	hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");

	CSceneMgr::Get_Instance()->Render(hBackDC);	

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);	
}

void CMainGame::Release()
{
	CTileMgr::Destroy();
	CBmpMgr::Destroy();
	CScrollMgr::Destroy();
	CKeyMgr::Destroy();
	CLineMgr::Destroy();
	CSceneMgr::Destroy();
	CObjMgr::Destroy();
		
	ReleaseDC(g_hWnd, m_hDC);
}
