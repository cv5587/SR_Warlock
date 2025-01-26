#include "..\..\Header\SoundMgr.h"
#include "..\..\Header\Timer.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
}


CSoundMgr::~CSoundMgr()
{
	Free();
}

void CSoundMgr::Ready_Sound()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	//FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, NULL);

	NULL_CHECK_RETURN(m_pSystem);

	LoadSoundFile();

	Update_Listener({ 0.f, 2.1f, 0.f }, { 1.f, 0.f, 0.f }, {0.f, 1.f, 0.f});
	m_vPlayerPos = { 0.f, 2.1f, 0.f };
}

void CSoundMgr::Update_Listener(_vec3 vPos, _vec3 vAt, _vec3 vUp)
{
	m_vPlayerPos = vPos;
	//_vec3 vUp = { 0.f, 1.f, 0.f };
	FMOD_VECTOR position = { vPos.x, vPos.y, vPos.z };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { vAt.x, vAt.y, vAt.z };
	FMOD_VECTOR up = { vUp.x, vUp.y, vUp.z };
	
	FMOD_Channel_Set3DAttributes(m_pChannelArr[BGM], &position, &velocity);
	FMOD_System_Set3DListenerAttributes(m_pSystem, 0, &position, &velocity, &forward, &up);
	
	
	FMOD_System_Update(m_pSystem);
	
}


void CSoundMgr::Play_Sound(wstring pSoundKey, CHANNELID eID, float fVolume)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}


void CSoundMgr::Play_Sound(wstring pSoundKey, CHANNELID eID, float fVolume, bool bPlay = true)
{
	if (!bPlay)
	{
		map<wstring, FMOD_SOUND*>::iterator iter;

		iter = find_if(m_mapSound.begin(), m_mapSound.end(),
			[&](auto& iter)->bool
			{
				return pSoundKey == iter.first;
			});

		if (iter == m_mapSound.end())
			return;

		FMOD_BOOL play = FALSE;

		if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &play))
		{
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
			FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
		}

		FMOD_System_Update(m_pSystem);
	}
}


void CSoundMgr::Play_Sound(wstring pSoundKey, float fVolume, _vec3 vPos)
{
	if(10.f < D3DXVec3Length(&(vPos - m_vPlayerPos))) return;	//거리 10 이상일 때 그냥 함수 건너뜀
		
	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	//_vec3 vPos = pGameObject->Get_TransformCom_Pointer()->Get_Pos();
	FMOD_VECTOR position = { vPos.x, vPos.y, vPos.z };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	FMOD_Channel_Set3DAttributes(m_pChannelArr[m_iSoundIdx], &position, &velocity);

	FMOD_BOOL play = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[m_iSoundIdx], &play))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[m_iSoundIdx]);
		FMOD_Sound_Set3DMinMaxDistance(iter->second, 1.0f, 5.0f);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[m_iSoundIdx], fVolume);

	FMOD_System_Update(m_pSystem);
	
	if (9 > m_iSoundIdx)
	{
		m_iSoundIdx++;
	}
	else
	{
		m_iSoundIdx = 0;
	}
}

void CSoundMgr::Play_BGM(wstring pSoundKey, float fVolume)
{
	FMOD_BOOL play = FALSE;
	if (!FMOD_Channel_IsPlaying(m_pChannelArr[BGM], &play)) return;

	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}


void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::SetFrequency(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetFrequency(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);

}

void CSoundMgr::SetFrequency(float fFreq)
{

	if (!m_bReverse)
	{
		for (int i =0; i<int(MAXCHANNEL); ++i)
		{
			FMOD_Channel_GetFrequency(m_pChannelArr[i], &m_tInitFreqs[i]);

			FMOD_Channel_SetFrequency(m_pChannelArr[i], m_tInitFreqs[i] / fFreq);
		}
		m_bReverse = true;
	}
	else
	{
		_float f;

		for (int i = 0; i<int(MAXCHANNEL); ++i)
		{
			FMOD_Channel_GetFrequency(m_pChannelArr[i], &f);
			if (m_tInitFreqs[i] <= f)
			{
				FMOD_Channel_SetFrequency(m_pChannelArr[i], m_tInitFreqs[i] / fFreq);
			}
		}
	}



}

void CSoundMgr::InitFrequency()
{
	for (int i = 0; i<int(MAXCHANNEL); ++i)
	{
		FMOD_Channel_SetFrequency(m_pChannelArr[i], m_tInitFreqs[i]);
	}
}


void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	long handle = _findfirst("../../Sound/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_3D | FMOD_HARDWARE, 0, &pSound);
		//FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_3D | FMOD_SOFTWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			wstring pSoundKey(fd.name, fd.name + iLength - 1);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}

void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}