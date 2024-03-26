#pragma once
#include "Obj.h"
#include "Line.h"

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

public:
	void	SetBoxLine(CLine* boxLine) { m_boxLine = boxLine; };

private:
	void		Gravity();

	float				m_fPower;
	float				m_fTime;

private:
	DWORD				m_dwTime;
	CLine*				m_boxLine;
};

