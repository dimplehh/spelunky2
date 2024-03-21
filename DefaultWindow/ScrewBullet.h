#pragma once
#include "Obj.h"
class CScrewBullet : public CObj
{
public:
	CScrewBullet();
	virtual ~CScrewBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	POINT		m_tCenter;			// ȸ�� �߽���

	float		m_fRotAngle;		// �̻��� ȸ�� ����
	float		m_fRotSpeed;		// �̻��� ȸ�� �ӵ�
	float		m_fRotDistance;		// �������κ��� ������ �Ÿ�

	bool		m_bStart;
};

