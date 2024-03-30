#pragma once
#include "HoldObj.h"

//�⺻ ����� ä��, ���� ����� �� ���� ����� �ٲ��� ��. (x������ �� ���� ��ų ����)

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