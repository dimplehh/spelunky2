#pragma once
#include "Obj.h"

class CObstacle : public CObj
{
public:
	CObstacle();
	virtual ~CObstacle();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int		m_iMotion = 0;
};
