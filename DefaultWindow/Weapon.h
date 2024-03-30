#pragma once
#include "HoldObj.h"

//기본 무기는 채찍, 총을 들었을 때 공격 방식이 바뀌어야 함. (x눌렀을 때 공격 스킬 적용)

class CWeapon : public CHoldObj
{
public:
	CWeapon();
	virtual ~CWeapon();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool		Gravity();

	float				m_fPower;
	float				m_fTime;
};