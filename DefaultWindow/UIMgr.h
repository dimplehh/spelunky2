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

public:
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
};
