#pragma once
#include "Obj.h"

class CRope : public CObj
{
public:
	CRope();
	virtual ~CRope();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

