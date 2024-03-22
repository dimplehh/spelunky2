#pragma once

#include "Obj.h"

class CPlayer :	public CObj
{
public:								//����	//���� ��			// ���帲  //��	//�Ͼ
	enum STATE { IDLE, WALK, JUMP, DIZZY, DIE, LOOKUP, LOOKFRONT, KNEELDOWN, CRAWL, STANDUP, 
			GRAB, ATTACKED, ALMOSTFELL, ATTACK, ENTER, EXIT, LADDER, PUSH, ST_END };
							//������ ��								
public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void		Key_Input();
	void		Load_Line();
	void		Offset();
	void		Motion_Change();

private:

	POINT		m_tPosin;
	float		m_fDistance;

	bool		m_bJump;
	bool		m_bKneelDown; //���帰 ����

	float		m_fPower;
	float		m_fTime;

	STATE		m_ePreState;
	STATE		m_eCurState;


	// y = �Ŀ�(��) * sin(90) * �ð� - (9.8f * �ð� * �ð�) / 2

};





