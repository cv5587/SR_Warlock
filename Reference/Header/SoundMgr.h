#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include <array>

BEGIN(Engine)

class ENGINE_DLL CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Ready_Sound();
	void Update_Listener(_vec3 vPos, _vec3 vAt, _vec3 vUp);
	void Free();

public:
	void Play_Sound(wstring pSoundKey, CHANNELID eID, float fVolume);				//채널에서 플레이 중인거 무시하고 출력
	void Play_Sound(wstring pSoundKey, CHANNELID eID, float fVolume, bool bPlay);				//채널에서 이미 플레이 중이면 끝나고 출력
	void Play_Sound(wstring pSoundKey, float fVolume, _vec3 vPos);	//거리에 따른 음량 조절
	void Play_Sound(wstring pSoundKey, float fVolume, _vec3 vPos, _vec3 vPlayerPos, _vec3 vPlayerAt);
	//false 넣으면, 채널에서 이미 플레이 중인 채널일 시 출력 스킵
	void Play_BGM(wstring pSoundKey, float fVolume);								//SetFrequency
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	void SetFrequency(CHANNELID eID, float fVolume);
	void SetFrequency(float fFreq);

	void InitFrequency();

private:
	void LoadSoundFile();

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<wstring, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체
	FMOD_SYSTEM* m_pSystem;

	//CPlayer* m_pPlayer;
	_int		 m_iSoundIdx;
	_vec3		m_vPlayerPos;
	array<float, MAXCHANNEL> m_tInitFreqs;
	_bool		m_bReverse = false;
};

END