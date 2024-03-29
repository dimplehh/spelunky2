#pragma once

#include "UIIcon.h"

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();

public:
	void		Insert_UI(CUIIcon::UIID uiId, CUIIcon* pMyUI) { m_mapUI.insert({ uiId, pMyUI }); }
	void		Set_UINum(CUIIcon::UIID _uiId, int _iNum) { m_mapUI[_uiId]->Set_Num(_iNum); }
	int			Get_Time() { return m_iTime; }
	void		Set_Time(int _time) { m_iTime += _time; }
	void		Reset_Time() { m_iTime = 0; }

public:
	void	Late_Update();
	void	Release();

public:
	static CUIMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CUIMgr;

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

private:
	static CUIMgr* m_pInstance;
	map<CUIIcon::UIID, CUIIcon*>	m_mapUI;
	int		m_iTime = 0;
	DWORD	m_dwTime;
};
