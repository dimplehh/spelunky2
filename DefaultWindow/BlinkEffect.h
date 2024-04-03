#pragma once
#include "Effect.h"

class CBlinkEffect : public CEffect
{
public:
	CBlinkEffect();
	virtual ~CBlinkEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};