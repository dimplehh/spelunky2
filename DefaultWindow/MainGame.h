#pragma once

#include "Define.h"

#include "Player.h"
#include "Mouse.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();		// 실시간으로(매 프레임마다) 변경되는 값을 만드는 함수
	void		Late_Update();
	void		Render();		// 실시간으로(매 프레임마다) 출력하는 함수
	void		Release();

private:
	HDC			m_hDC;

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[32];
};

// 디자인 패턴 : 코드의 구조들을 일정한 형태로 만들어 재사용하기 편리하게 만든 패턴
// https://refactoring.guru/ko/design-patterns

// 추상 팩토리 패턴 : 관련 객체들의 구상 클래스들을 지정하지 않고도 관련 객체들의 모음을 생성할 수 있도록 하는 생성패턴입니다.
// 객체 생성 시 동반되는 공통적인 작업을 추상화 시킨 것

// 반복자 패턴 : 내부 표현 방식은 공개하지 않고 순차적인 접근 방법을 제공하는 패턴

// 메디에이터(중재자) 패턴 : 상호작용하는 객체들이 서로 복잡한 관계를 맺고 있을 경우 별도의 형식으로 정의하여 중재하는 객체를 두는 패턴

// 싱글톤 패턴 : 단 하나의 인스턴스만 생성하여 사용하는 패턴

// 스테이트 패턴 : FSM(유한 상태 기계) 기반으로 하는 객체의 상태를 표현하는 패턴, 자신이 취할 수 있는 유한한 개수의 상태를 가지며 그 중 반드시 하나의 상태만을 취하게 만드는 패턴