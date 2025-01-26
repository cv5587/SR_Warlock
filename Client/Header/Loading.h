#pragma once

#include "Base.h"
#include "Engine_Define.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID			Get_LoadingID() const { return m_eID; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() const { return m_bFinish; }

public:
	HRESULT		Ready_Loading(LOADINGID eID);
	_uint		Loading_ForStage();
	wstring		Get_String() const { return m_szLoading; }
public:
	static unsigned int CALLBACK Thread_Main(void* pArg);


private:
	_uint	Add_PropTexture(); //Prop 텍스처 추가 함수 모음
	_uint	Add_MonsterTexture(); //Monster 텍스처 추가 함수 모음
	_uint	Add_MonsterBulletTexture();
	_uint	Add_EffectTexture();
	


	_uint	Add_Player_Things();
	_uint	Add_Map_Things();

	_uint	Add_Environmental_Things();

	_uint	Add_Items();



private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;

	LOADINGID			m_eID;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	bool				m_bFinish;
	wstring			m_szLoading;

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void		Free();

};

