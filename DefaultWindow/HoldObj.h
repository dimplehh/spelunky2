#pragma once
#include "Obj.h"

class CHoldObj : public CObj
{
public:
	enum HOLDOBJID { HOLDOBJ_JAR, HOLDOBJ_KEY, HOLDOBJ_STONE, HOLDOBJ_END };

public:
	CHoldObj();
	virtual ~CHoldObj();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void		Set_HoldObjID(HOLDOBJID _eId) { m_eHoldObjID = _eId; }
	HOLDOBJID	Get_HoldObjID() { return m_eHoldObjID; }

	bool		GetThrowing() { return m_bThrowing; }

private:
	bool		Gravity();

	float				m_fPower;
	float				m_fTime;

	float				m_fThrowTime;

private:
	HOLDOBJID			m_eHoldObjID;
	bool				m_bIsActive = false;
	bool				m_bThrowing = false;

private:
	DWORD				m_dwTime;
};