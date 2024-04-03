#pragma once
#include "Obj.h"

class CLava : public CObj
{
public:
	CLava();
	virtual ~CLava();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

