#pragma once
#include "Obj.h"
class CBox : public CObj
{
public:
	CBox();
	virtual ~CBox();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void		Gravity();

	float				m_fPower;
	float				m_fTime;

private:
	DWORD				m_dwTime;
};

