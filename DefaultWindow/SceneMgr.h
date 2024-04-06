#pragma once

#include "Define.h"

#include "Logo.h"
#include "MyEdit.h"

#include "MyMenu.h"
#include "Stage.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Ending.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	static CSceneMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}
	static void		Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	SCENEID			Get_Scene() { return m_eCurScene; }
	CScene*			GetRealScene() { return m_pScene; }

public:
	void			Scene_Change(SCENEID eID);
	void			Scene_Change(SCENEID eID, int _money, int _time);
	void			Update();
	void			Late_Update();
	void			Render(HDC hDC);
	void			Release();

public:
	void			SetTotalTime(int _time) { m_iTotalTime += _time; };
	void			SetTotalMoney(int _money) { m_iTotalMoney += _money; };
	int				GetTotalTime() { return m_iTotalTime; };
	int				GetTotalMoney() { return m_iTotalMoney; }


private:
	static CSceneMgr*		m_pInstance;
	CScene*					m_pScene;

	SCENEID					m_ePreScene;
	SCENEID					m_eCurScene;

	int						m_iTotalTime = 0;
	int						m_iTotalMoney = 0;
};

