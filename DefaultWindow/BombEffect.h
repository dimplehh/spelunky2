#pragma once
#include "Effect.h"

class CBombEffect : public CEffect
{
public:
	CBombEffect();
	virtual ~CBombEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	DWORD	m_dwTime;
	bool	m_bCheckFirstTime = true;
	int		m_iMaxSize = 45;
	int		m_iMinSize = 30;
	float	m_fAddX = 0;
	float	m_fAddY = 0;
	float	m_fStartX = 0;
	float	m_fStartY = 0;
	int		m_iAlpha = 0;
	int		m_iSpeed = 1;
};