#pragma once
#include "Effect.h"

class CDustEffect : public CEffect
{
public:
	CDustEffect();
	virtual ~CDustEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};
