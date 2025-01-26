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
	void Play_Sound(wstring pSoundKey, CHANNELID eID, float fVolume);				//ä�ο��� �÷��� ���ΰ� �����ϰ� ���
	void Play_Sound(wstring pSoundKey, CHANNELID eID, float fVolume, bool bPlay);				//ä�ο��� �̹� �÷��� ���̸� ������ ���
	void Play_Sound(wstring pSoundKey, float fVolume, _vec3 vPos);	//�Ÿ��� ���� ���� ����
	void Play_Sound(wstring pSoundKey, float fVolume, _vec3 vPos, _vec3 vPlayerPos, _vec3 vPlayerAt);
	//false ������, ä�ο��� �̹� �÷��� ���� ä���� �� ��� ��ŵ
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
	// ���� ���ҽ� ������ ���� ��ü 
	map<wstring, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü
	FMOD_SYSTEM* m_pSystem;

	//CPlayer* m_pPlayer;
	_int		 m_iSoundIdx;
	_vec3		m_vPlayerPos;
	array<float, MAXCHANNEL> m_tInitFreqs;
	_bool		m_bReverse = false;
};

END