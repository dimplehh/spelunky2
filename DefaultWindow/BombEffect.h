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
};