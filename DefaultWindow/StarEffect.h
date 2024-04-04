#pragma once
#include "Effect.h"

class CStarEffect : public CEffect
{
public:
	CStarEffect();
	virtual ~CStarEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	DWORD	m_dwTime;
	bool	m_bCheckFirstTime = true;
	int		m_iMaxSize = 15;
	int		m_iMinSize = 5;
	float	m_fAddX = 0;
	float	m_fAddY = 0;
};