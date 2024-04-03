#pragma once
#include "Obj.h"

class CEffect :public CObj
{
public:
	enum EFFECTID {EFFECT_DUST, EFFECT_STAR, EFFECT_BLINK, EFFECT_STONE, EFFECT_BOMB, EFFECT_END};

public:
	CEffect();
	~CEffect();

	void		SetActive(bool _active) { m_bIsActive = _active; }
	bool		GetActive() { return m_bIsActive; }
	EFFECTID	GetEffectID() { return m_eEffectID; }

protected:
	EFFECTID	m_eEffectID = EFFECT_END;
	int			m_iSize		= 0;
	bool		m_bIsActive = false;
};
