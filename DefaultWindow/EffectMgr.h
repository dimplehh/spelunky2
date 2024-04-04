#pragma once

#include "ObjMgr.h"
#include "Effect.h"

class CEffectMgr
{
private:
	CEffectMgr();
	~CEffectMgr();

public:
	void	Initialize();
	void	ActiveEffect(CEffect::EFFECTID _id, float _fX, float _fY);
	bool	GetEffectEnd(CEffect::EFFECTID _id);

public:
	static CEffectMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CEffectMgr;

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
	static CEffectMgr* m_pInstance;
	list<CObj*>*		m_pEffectList = nullptr;
};
