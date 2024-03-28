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

public:
	void	SetRopeSize(int _ropeSize) { m_iRopeSize = _ropeSize; }
	int		GetRopeSize() { return m_iRopeSize; }
	void	CheckMoveEnd();	

private:
	int		m_iRopeSize = 0;
	int		m_iRenderIndex = 1;
};

