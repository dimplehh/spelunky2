#pragma once
#include "Effect.h"

class CStoneEffect : public CEffect
{
public:
	CStoneEffect();
	virtual ~CStoneEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	DWORD	m_dwTime;
	bool	m_bCheckFirstTime = true;
	float	m_fPreY = 0;
	float	m_fAddX = 0;
	float	m_fTime = 0;
};