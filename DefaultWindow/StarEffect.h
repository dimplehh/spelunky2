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
};