#pragma once
#include "Obj.h"

class CHoldObj : public CObj
{
public:
	enum HOLDOBJID { HOLDOBJ_JAR, HOLDOBJ_KEY, HOLDOBJ_GUN, HOLDOBJ_END };

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

	void		SetOwner(CObj* _pOwner) { m_pOwner = _pOwner; }
	CObj*		GetOwner() { return m_pOwner; }

private:
	bool		Gravity();

	float				m_fPower;
	float				m_fTime;

private:
	HOLDOBJID			m_eHoldObjID;

	CObj* m_pOwner;
};