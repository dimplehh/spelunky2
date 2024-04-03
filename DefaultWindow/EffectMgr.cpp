#include "stdafx.h"
#include "EffectMgr.h"
#include "ObjMgr.h"

CEffectMgr* CEffectMgr::m_pInstance = nullptr;

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::Initialize()
{
	m_pEffectList = CObjMgr::Get_Instance()->GetEffectList();
}

void CEffectMgr::ActiveEffect(CEffect::EFFECTID _id, float _fX, float _fY)
{

	for (auto iter = m_pEffectList->begin(); iter != m_pEffectList->end(); iter++)
	{

		if (dynamic_cast<CEffect*>(*iter)->GetEffectID() == _id)
		{
			(*iter)->Set_Pos(_fX, _fY);
			dynamic_cast<CEffect*>(*iter)->SetActive(true);
		}
	}
}

