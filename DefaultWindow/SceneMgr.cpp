#include "stdafx.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Player.h"

CSceneMgr*		CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_pScene(nullptr), m_ePreScene(SC_END), m_eCurScene(SC_LOGO)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}


void CSceneMgr::Scene_Change(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_MENU:
			m_pScene = new CMyMenu;
			break;

		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;

		case SC_STAGE:
			m_pScene = new CStage;
			break;

		case SC_STAGE2:
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetRespawnPos(TILECX * (55 + 0.5f), TILECY * (10 + 0.5f));
			CObjMgr::Get_Instance()->Get_Player()->Set_Pos(TILECX * (55+ 0.5f), TILECY * (10 + 0.5f));
			m_pScene = new CStage2;
			break;
		case SC_STAGE3:
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->SetRespawnPos(TILECX * (4 + 0.5f), TILECY * (5 + 0.5f));
			CObjMgr::Get_Instance()->Get_Player()->Set_Pos(TILECX * (4 + 0.5f), TILECY * (5 + 0.5f));
			m_pScene = new CStage3;
			break;
		}

		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}


void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
