#include "stdafx.h"
#include "..\Header\Loading.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Engine_Define.h"
#include "GameData.h"

#include <set>



CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL,	// 디폴트 보안 속성
		0,		// 디폴트 스택 사이즈(1바이트)
		Thread_Main, // 동작 시킬 쓰레드 함수 이름(__stdcall 규약)
		this,		// 쓰레드 함수의 매개 변수로 전달할 값
		0,			// 쓰레드 생성 및 실행을 조절하기 위한 FLAG
		NULL);		// 쓰레드 ID 반환

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	Add_Environmental_Things();



	Add_Player_Things();
	Add_PropTexture();
	Add_MonsterTexture();
	Add_MonsterBulletTexture();
	Add_EffectTexture();
	Add_Items();

	Add_Map_Things();

	m_bFinish = true;

	(m_szLoading= L"Loading Complete");

	return 0;
}


_uint CLoading::Add_EffectTexture()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_OverkillTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/overkill_%d.png", 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_FiredeathTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/death_fire%d.png", 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_MagicdeathTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/death_magic%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_MagicboomTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/magic_boom%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_MagicblastTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/magicblast_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Effect_Bullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Bullet/Effect_Bullet%d.png", 15)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Effect_Explosion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Explosion/Explosion%d.png", 9)), E_FAIL);

	//Blood
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Blood_Red1Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/bloodhit_2_big000%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Blood_Red2Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/bloodhit_big000%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Blood_YellowTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/bloodhit2_big_yellow000%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Blood_GreenTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/bloodhit2_big_green000%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Blood_BlackTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/bloodhit2_big_black000%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Puddle_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/puddle%d.png", 4)), E_FAIL);


	//Explosion
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Expl_1Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/explosion0_ground%d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Expl_2Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/explosion1_ground%d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FX_Expl_3Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/explosion2_ground%d.png", 13)), E_FAIL);

	//FIre
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FireTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/EffectObj/fire_ground_%d.png", 7)), E_FAIL);
	return S_OK;
}

_uint CLoading::Add_PropTexture()
{
	//Prop 텍스처 추가

	//Door
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorNormalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_blastdoor_0000.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorGreyTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0003.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorRedTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0000.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorYellowTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0001.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorBlueTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0002.png")), E_FAIL);

	//Portal
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PortalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/portal_entry_dark_blue000%d.png", 8)), E_FAIL);

	//Exit
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/wormhole000%d.png", 4)), E_FAIL);

	//Toggle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleNormalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_normal000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleRedTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_red000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleYellowTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_yellow000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleBlueTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_blue000%d.png", 2)), E_FAIL);

	//Box
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxNormalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/box000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxExplTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/box_expl000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Broken_WallTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/broken_wall_lab1_000%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Broken_DoorTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/broken_blastdoor000%d.png", 8)), E_FAIL);

	//Pillar
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PillarTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/1broken_base_pillar000%d.png", 6)), E_FAIL);

	//Thorn
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ThornTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/thorn_broken000%d.png", 4)), E_FAIL);


	//ShopNPC
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShopNPCTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/NPC/ShopNPC1.png")), E_FAIL);

	//Lamp
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LampTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/InActiveObj/lamp.png")), E_FAIL);

	//Lightpost
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LightpostTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/InActiveObj/lightpost.png")), E_FAIL);

	//Corpse
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CorpseTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/InActiveObj/corpse.png")), E_FAIL);

	return S_OK;
}

_uint CLoading::Add_MonsterTexture()
{
	//Yeti
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_idle.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiRunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_run%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_slash%d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_death_run%d.png", 13)), E_FAIL);

	//Looker
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_back%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerRunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_float%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_attack%d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_death%d.png", 15)), E_FAIL);

	//Flamer
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_back.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerRunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_walk%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_attack%d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/death_fire%d.png", 22)), E_FAIL);


	//Shambler
//Shambler_Normal
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_back.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_RunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_walk%d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_AtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_slash%d.png", 14)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_BashTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_bash%d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_MagicTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_magic%d.png", 12)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_HitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_hit.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_DeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_damage_first%d.png", 6)), E_FAIL);

	//Shambler_Halfcut
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerHalfcut_RunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Halfcut/newshambler_damage_first_walk%d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerHalfcut_AtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Halfcut/newshambler_damaged_first_slash%d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerHalfcut_MagicTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Halfcut/newshambler_damaged_first_cast%d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerHalfcut_HitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Halfcut/newshambler_damage_first_hit.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerHalfcut_DeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Halfcut/newshambler_damage_second%d.png", 7)), E_FAIL);


	//Shambler_Fullcut
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerFullcut_RunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Fullcut/newshambler_damage_second_walk%d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerFullcut_AtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Fullcut/newshambler_damage_second_chomp%d.png", 10)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerFullcut_HitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Fullcut/newshambler_damage_second_hit.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerFullcut_DeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Fullcut/newshambler_damage_second_death%d.png", 13)), E_FAIL);

	//Boss Phase 1

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase1_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_1.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase1_AtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_1_fragile%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase1_DeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_1_death%d.png", 20)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase1_HitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_1_hit.png")), E_FAIL);


	//Boss Phase 2
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase2_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_idle%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase2_AtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_attack%d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Phase2_DeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss/cthulhu_death%d.png", 8)), E_FAIL);


	//Boss Tentacle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Tentacle_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss_tentacle/cthulhu_tentacle_idle.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Tentacle_InTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss_tentacle/cthulhu_tentacle_in%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Tentacle_OutTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss_tentacle/cthulhu_tentacle_out%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Tentacle_AtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss_tentacle/cthulhu_tentacle_shoot%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_Tentacle_DeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Boss_tentacle/cthulhu_tentacle_death%d.png", 12)), E_FAIL);

	//ExitMonster
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitMonsterIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Exit/exit_idle%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitMonsterHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Exit/exit_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitMonsterAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Exit/exit_attack%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitMonsterDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Exit/exit_death%d.png", 25)), E_FAIL);


	//Thingy
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_idle.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_HitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_MorphTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_morph%d.png",18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_WalkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_walk%d.png",6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_DeathTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_death%d.png",10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackATexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_attack_A%d.png",18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackBTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_attack_B%d.png",28)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_attack%d.png",14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_SpikeTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_spikes%d.png",20)), E_FAIL);


	return S_OK;
}

_uint CLoading::Add_Player_Things()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Claymore", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Claymore/Claymore%d.png", 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Minigun", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Minigun/Minigun%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_TurboMinigun", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Minigun/TurboMinigun%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Revolver", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Revolver/Revolver%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Magnum", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Revolver/Magnum%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Rocket_Launcher", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Rocket_Launcher/Rocket_Launcher%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Smoke", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Smoke0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Napalm_Launcher", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Rocket_Launcher/Napalm_Launcher%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_MiniShotgun", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Shotgun/MiniShotgun%d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Shotgun", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Shotgun/Shotgun%d.png", 27)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Staff", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Staff/Staff%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Reactor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Reactor/Reactor%d.png", 37)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BulletCase", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bullet/BulletCase0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PBullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bullet/PBullet%d.png", 20)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIWeapon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Weapon/UIWeapon%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HUD", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/HUD/HUD%d.png", 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Face", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Face/Face%d.png", 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Key", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Key/Key%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Shop/Shop%d.png", 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cursor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Cursor/Cursor%d.png", 3)), E_FAIL);

	return _uint();
}

_uint CLoading::Add_Map_Things()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_lab_floor_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_lab_wall_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_lab_ceiling_%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture3", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_glass.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture4", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_base_doorframe_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture5", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_antarctic_%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture6", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_black_goo000%d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture7", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_boss_wall_%d.png", 5)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WhiteMiniMapTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MiniMap/White256x256.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GrayMiniMapTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MiniMap/Gray256x256.jpg")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YellowDotTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MiniMap/YellowDot.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RedSpot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Red_spot.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WhiteSpot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/White_spot.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YellowSpot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Yellow_spot.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PurpleSpot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Purple_spot.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GibYeti", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Gib/gib_yeti_%d.png",3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GibFlamer", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Gib/gib_flamer_%d.png",2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GibLooker", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Gib/gib_looker_%d.png",4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GibShambler", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Gib/newshambler_gib_%d.png",3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GibThingy", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Particle/Gib/gib_thing_%d.png",4)), E_FAIL);

	

	CGameData::Create(m_pGraphicDev);
	CGameData::GetInstance()->Set_Map_Option(L"../Bin/Resource/Data/Stage01.txt", L"../Bin/Resource/Mesh/ii.fbx", L"../Bin/Resource/Mesh/iid.fbx");
	CGameData::GetInstance()->Set_Map_Option(L"../Bin/Resource/Data/Stage02.txt", L"../Bin/Resource/Mesh/boss.fbx", L"../Bin/Resource/Mesh/Dboss.fbx");
	return S_OK;
}

_uint CLoading::Add_Environmental_Things()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger4.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Particle", CParticleBuffer::Create(m_pGraphicDev)), E_FAIL);
	return S_OK;
}

_uint CLoading::Add_Items()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Bullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Bullet/Bullet%d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Exp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Exp/Exp%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Health", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Health/Health%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Key", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Key/Key%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Mana", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Mana/Mana%d.png", 2)), E_FAIL);

	return S_OK;
}

_uint CLoading::Add_MonsterBulletTexture()
{
	//Looker Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/tentacle_monster_projectile%d.png", 5)), E_FAIL);

	//Flamer Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/fire_monster_%d.png", 8)), E_FAIL);

	//Slow Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SlowBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/slow_bullet%d.png", 9)), E_FAIL);

	//Magic Bullet(Boss)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MagicBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/cthulhu_tentacle_missile%d.png", 14)), E_FAIL);

	//Ice Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_IceBulletATexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/icespike/ice_spike_a%d.png", 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_IceBulletBTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/icespike/ice_spike_b%d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_IceBulletCTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/icespike/ice_spike_c%d.png", 18)), E_FAIL);

	//Boss 2 Spike Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss2SpikeTextureA", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/icespike/hellboss_final_spike_01_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss2SpikeTextureB", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/icespike/hellboss_final_spike_02_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss2SpikeTextureC", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/icespike/hellboss_final_tentacle%d.png", 18)), E_FAIL);

	//Spiral Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiralBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/spiral_bullet000%d.png", 8)), E_FAIL);

	return S_OK;
}




unsigned int CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);


	Safe_Release(m_pGraphicDev);
}
