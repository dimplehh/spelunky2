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

};