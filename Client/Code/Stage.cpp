#include "stdafx.h"
#include "..\Header\Stage.h"
#include "..\Header\Boss_Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"
#include "Monster.h"
#include "BackGround.h"
#include "Terrain.h"
#include "PlayerUICamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "TmpMesh.h"
#include "MeshGroup.h"
#include "LineList.h"

#include "Item.h"
#include "HUDUICamera.h"
#include "BarUICamera.h"
#include "FaceUICamera.h"
#include "KeyUICamera.h"
#include "ScreenEffect.h"

#include "Door.h"
#include "Portal.h"
#include "Exit.h"
#include "Box.h"
#include "Thorn.h"
#include "Toggle.h"
#include "Yeti.h"
#include "Fire.h"
#include "Looker.h"
#include "Flamer.h"
#include "Shambler.h"
#include "Boss_Phase1.h"
#include "Boss_Phase2.h"
#include "Boss_Tentacle.h"
#include "Snow.h"
#include "ExitMonster.h"

#include "ShopUICamera.h"
#include "Cursor.h"
#include "GameData.h"
#include "SpawnToggle.h"

#include "MainMenu.h"
#include "DecorativeProp.h"
#include "ShopNPC.h"
#include "Thingy.h"
#include "MiniMap.h"

#include "Light.h"
#include "LightToggle.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev), m_pPlayer(nullptr), m_teleport(false)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	

	Engine::Get_SoundMgr()->StopAll();
	Engine::Get_SoundMgr()->Play_BGM(L"BGM_Stage.wav", 0.3f);

	return S_OK;
}
Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{	
	m_pPlayer->Set_Nearby(false);
	return __super::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(const float& fTimeDelta)
{
	__super::LateUpdate_Scene(fTimeDelta);

	Engine::LateUpdate_CollisionMgr(fTimeDelta);

	//teleport 코드
	if (m_pExit->Is_Teleport_Pos())
	{

		CGameData::GetInstance()->Clear_LineCollision(0);
		CLightMgr::DestroyInstance();
		CSoundMgr::GetInstance()->StopAll();
		Engine::Get_SoundMgr()->Play_Sound(L"Teleporter_Enter.wav", PROPSOUND, 0.5f);
		CMiniMap::DestroyInstance();

		m_teleport = true;
		Engine::CScene* pScene = nullptr;

		pScene = CBossStage::Create(m_pGraphicDev,*m_pPlayer);
		NULL_CHECK_RETURN(pScene);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene));
		return;
	}
		
}

void CStage::Render_Scene()
{

	const _tchar* szProtoName = L"Player Pos: (X : %.2f, Y : %.2f, Z: %.2f) ";
	const _vec3 vPlayerPos = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

	TCHAR	szPlayerPos[128] = L"";

	swprintf_s(szPlayerPos, szProtoName, vPlayerPos.x, vPlayerPos.y , vPlayerPos.z);

	Render_Font(L"Font_Soo", wstring(szPlayerPos), &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	const _tchar* szEyePos_pre = L"Player Pos: (X : %.2f, Y : %.2f, Z: %.2f) ";
	const _vec3 vEyePos = m_pPlayer->Get_Player_Eye();

	TCHAR	szEyePos[128] = L"";

	swprintf_s(szEyePos, szEyePos_pre, vEyePos.x, vEyePos.y, vEyePos.z);

	Render_Font(L"Font_Soo", wstring(szEyePos), &_vec2(10.f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	const _tchar* szDir = L"Player Direction : %.2f %.2f %.2f  ";
	const _vec3 vDir = m_pPlayer->Get_Dir();

	TCHAR	szPlayerDir[128] = L"";

	swprintf_s(szPlayerDir, szDir, vDir.x, vDir.y, vDir.z);

	Render_Font(L"Font_Soo", wstring(szPlayerDir), &_vec2(10.f, 70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


	const _tchar* szoriginDir = L"Player Direction : %.2f %.2f %.2f  ";
	const _vec3 voriginDir = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - m_pPlayer->Get_PrePos();

	TCHAR	szPlayeroriginDir[128] = L"";

	swprintf_s(szPlayeroriginDir, szoriginDir, voriginDir.x, voriginDir.y, voriginDir.z);

	Render_Font(L"Font_Soo", wstring(szPlayeroriginDir), &_vec2(10.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Engine::DrawMesh();
	// DEBUG
}

HRESULT CStage::Ready_Layer_Environment(wstring pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	Engine::CGameObject*		pGameObject = nullptr;

	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);
		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(wstring pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev,
		//2.1f
		&_vec3(-2.f, 2.1f, -1.f),
		&_vec3(0.f, D3DXToRadian(180.f), 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	Engine::Set_Camera(static_cast<Engine::CCamera*>(pGameObject));

	const multimap<std::wstring, CGameObject*>& tmpObjGroup = CGameData::GetInstance()->Get_TotalGroup(0);

	for (auto& iter : tmpObjGroup)
	{
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(iter.first, iter.second), E_FAIL);
		iter.second->Push_CollisionMgr();
	}
	CGameData::GetInstance()->Set_IsCloneTotalGroups(0);


	CMeshGroup* tmpMeshGroup = CGameData::GetInstance()->Get_MeshGroup(0);
	tmpMeshGroup->Set_Is_Clone(true);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MeshGroup", tmpMeshGroup));

	CGameData::GetInstance()->Activate_LineCollision(0);


	FAILED_CHECK_RETURN(CMiniMap::Create(m_pGraphicDev, m_pPlayer, CGameData::GetInstance()->Get_FloorMeshIndices(0)), E_FAIL)


	D3DXVECTOR3 vPos = {-2.f, 2.1f, -1.f};
	D3DXVECTOR3 lowerBound = vPos - D3DXVECTOR3{ 2.f,2.f,2.f };
	D3DXVECTOR3 upperBound = vPos + D3DXVECTOR3{ 2.f,2.f,2.f };


	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN0, vPos ,lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);


	vPos = { -14.f, 2.1f, -11.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };

	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN1, vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);

	vPos = { -14.f, 2.1f, -11.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };

	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN4, vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);


	vPos = { -15.f, 2.1f, 0.f };
	lowerBound = vPos - D3DXVECTOR3{ 2.f,2.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 2.f,2.f,2.f };

	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN1, vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);


	vPos = { 3.f, 2.1f, 3.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };

	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN2, vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);

	vPos = { -19.f, 2.1f, 11.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };

	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN5, vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);

	vPos = { -6.f, 2.1f, 23.f };
	lowerBound = vPos - D3DXVECTOR3{ 2.5f,2.5f,2.5f };
	upperBound = vPos + D3DXVECTOR3{ 2.5f,2.5f,2.5f };

	pGameObject = CSpawnToggle::Create(m_pGraphicDev, SPAWN7, vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpawnToggle", pGameObject), E_FAIL);

	pGameObject = CThingy::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Thingy", pGameObject), E_FAIL);


	//pGameObject = CLineList::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LineList", pGameObject), E_FAIL);

	//pGameObject = CTmpMesh::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TmpMesh", pGameObject), E_FAIL);

	//pGameObject = CMeshGroup::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MeshGroup", pGameObject), E_FAIL);

	//pGameObject = CItem::Create(m_pGraphicDev, {10.f, 2.f, -6.f}, BULLET, 0, 10);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet", pGameObject), E_FAIL);

	//pGameObject = CItem::Create(m_pGraphicDev, { 10.f, 2.f, -2.f }, EXP, 0, 1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Exp", pGameObject), E_FAIL);

	//pGameObject = CItem::Create(m_pGraphicDev, { 10.f, 2.f, 2.f }, HEALTH, 0, 10);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Health", pGameObject), E_FAIL);

	//pGameObject = CItem::Create(m_pGraphicDev, { 10.f, 2.f, 6.f }, KEY, 1, 0);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"YellowKey", pGameObject), E_FAIL);

	//pGameObject = CItem::Create(m_pGraphicDev, { 10.f, 2.f, 10.f }, MANA, 0, 10);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mana", pGameObject), E_FAIL);

	//pGameObject = CDoor::Create(m_pGraphicDev, DOOR_GREY); //Door, Portal은 플레이어보다 늦게 생성해야됨
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Door", pGameObject), E_FAIL);

	//pGameObject = CDoor::Create(m_pGraphicDev, DOOR_RED);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Door_Red", pGameObject), E_FAIL);

	pGameObject = CShopNPC::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShopNPC", pGameObject), E_FAIL);

	pGameObject = m_pExit = CExit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Exit", pGameObject), E_FAIL);

	//pGameObject = CBox::Create(m_pGraphicDev, PILLAR);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Box", pGameObject), E_FAIL);

	//pGameObject = CToggle::Create(m_pGraphicDev, TOG_RED);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Toggle", pGameObject), E_FAIL);

	//pGameObject = CPortal::Create(m_pGraphicDev, PORTAL_A);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Portal", pGameObject), E_FAIL);
	//pGameObject = CExit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Exit", pGameObject), E_FAIL);
	//pGameObject = CThorn::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Thorn", pGameObject), E_FAIL);

	//pGameObject = CYeti::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Yeti", pGameObject), E_FAIL);

	//pGameObject = CLooker::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Looker", pGameObject), E_FAIL);

	//pGameObject = CFlamer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Flamer", pGameObject), E_FAIL);

	//pGameObject = CShambler::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shambler", pGameObject), E_FAIL);

	//pGameObject = CBoss_Phase1::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss1", pGameObject), E_FAIL);


	//pGameObject = CBoss_Phase2::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss2", pGameObject), E_FAIL);


	//for (_int i = 0; i < 50; ++i)
	//{
	//	pGameObject = CEffect::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
	//}


	//자동문
	pGameObject = CDoor::Create(m_pGraphicDev, DOOR_AUTO, { -9.f, 2.f, -4.f }, D3DXToRadian(90.f), SPAWN_NONE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorAuto", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	pGameObject = CDoor::Create(m_pGraphicDev, DOOR_AUTO, { -9.f, 2.f, -6.f }, D3DXToRadian(90.f), SPAWN_NONE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorAuto", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	pGameObject = CDoor::Create(m_pGraphicDev, DOOR_AUTO, { -10.f, 2.f, -6.f }, D3DXToRadian(0.f), SPAWN_NONE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorAuto", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	pGameObject = CDoor::Create(m_pGraphicDev, DOOR_AUTO, { -12.f, 2.f, -6.f }, D3DXToRadian(0.f), SPAWN_NONE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorAuto", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	pGameObject = CDoor::Create(m_pGraphicDev, DOOR_AUTO, { -16.f, 2.f, -3.f }, D3DXToRadian(90.f), SPAWN_NONE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorAuto", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	pGameObject = CDoor::Create(m_pGraphicDev, DOOR_AUTO, { -16.f, 2.f, -5.f }, D3DXToRadian(90.f), SPAWN_NONE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorAuto", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	//부수는문

	pGameObject = CBox::Create(m_pGraphicDev, BROKEN_WALL, TRUE, 90.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Brokenwall", pGameObject), E_FAIL);
	pGameObject->Get_TransformCom_Pointer()->Set_Pos(2.f, 2.f, 12.f);
	pGameObject->Push_CollisionMgr();

	pGameObject = CBox::Create(m_pGraphicDev, BROKEN_WALL, TRUE, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Brokenwall", pGameObject), E_FAIL);
	pGameObject->Get_TransformCom_Pointer()->Set_Pos(-15.f, 2.f, 13.f);
	pGameObject->Push_CollisionMgr();

	pGameObject = CBox::Create(m_pGraphicDev, BROKEN_WALL, TRUE, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Brokenwall", pGameObject), E_FAIL);
	pGameObject->Get_TransformCom_Pointer()->Set_Pos(-21.f, 2.f, -6.f);
	pGameObject->Push_CollisionMgr();

	pGameObject = CBox::Create(m_pGraphicDev, BROKEN_DOOR, TRUE, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BrokenDoor", pGameObject), E_FAIL);
	pGameObject->Get_TransformCom_Pointer()->Set_Pos(-11.f, 2.f, -9.f);
	pGameObject->Push_CollisionMgr();

	//포탈
	pGameObject = CPortal::Create(m_pGraphicDev, PORTAL_A);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PortalA", pGameObject), E_FAIL);
	pGameObject->Get_TransformCom_Pointer()->Set_Pos({ -27.f, 2.1f, 35.f });

	pGameObject = CPortal::Create(m_pGraphicDev, PORTAL_B);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PortalB", pGameObject), E_FAIL);
	pGameObject->Get_TransformCom_Pointer()->Set_Pos({ -25.f, 2.1f, -3.f });

	////Exit monster
	pGameObject = CExitMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ExitMonster", pGameObject), E_FAIL);
	pGameObject->Push_CollisionMgr();

	vector<CLight*> lightGroup[30];
	CDecorativeProp* pDecoProp = nullptr;
	//0
	vPos = { -16.f, 2.4f, -11.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos,5.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[0].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[1].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[2].emplace_back(pDecoProp->Get_Light_Ptr());
	//lightGroup[3].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[4].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[5].emplace_back(pDecoProp->Get_Light_Ptr());

	//1
	vPos = { -9.f, 2.4f, -6.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[0].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[2].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[3].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[6].emplace_back(pDecoProp->Get_Light_Ptr());

	//2
	vPos = { -20.f, 2.4f, -2.5f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[1].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[4].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[5].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());

	//3
	vPos = { -25.f, 2.4f, -3.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP,vPos, 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[1].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[4].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[5].emplace_back(pDecoProp->Get_Light_Ptr());


	//4 노란색
	vPos = { -15.f, 2.4f, -1.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 2.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[3].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[6].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[8].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[9].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트그룹추가
	lightGroup[2].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());



	//5
	vPos = { -10.f, 2.4f, 0.5f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[6].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[8].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[9].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[13].emplace_back(pDecoProp->Get_Light_Ptr());


	//6
	vPos = { -2.5f, 2.4f, 0.5f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[4].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[6].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[8].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[9].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 그룹 추가
	lightGroup[15].emplace_back(pDecoProp->Get_Light_Ptr());


	//7
	vPos = { -15.f, 2.4f, 4.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[4].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[8].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[9].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[13].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 그룹 추가
	lightGroup[1].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[5].emplace_back(pDecoProp->Get_Light_Ptr());



	//8
	vPos = { -10.f, 2.4f, 6.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 2.5f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[8].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[9].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[13].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[14].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[15].emplace_back(pDecoProp->Get_Light_Ptr());


	//9 흰색
	vPos = { -5.f, 2.4f, 7.5f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO::라이트 그룹 추가
	lightGroup[9].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[13].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[14].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[15].emplace_back(pDecoProp->Get_Light_Ptr());

	//10 노란색
	vPos = { -19.f, 2.4f, 8.5f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 2.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[4].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[11].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[16].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[17].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 그룹 추가
	lightGroup[7].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[8].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[10].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[12].emplace_back(pDecoProp->Get_Light_Ptr());


	//11
	vPos = { -24.f, 2.4f, 11.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[16].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[17].emplace_back(pDecoProp->Get_Light_Ptr());


	//12
	vPos = { -29.f, 2.4f, 11.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[16].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[17].emplace_back(pDecoProp->Get_Light_Ptr());

	//13
	vPos = { -9.25f, 2.4f, 14.00f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.7f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[16].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[17].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[18].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[19].emplace_back(pDecoProp->Get_Light_Ptr());


	//14
	vPos = { 0.f, 2.4f, 16.00f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 4.5f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[18].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[19].emplace_back(pDecoProp->Get_Light_Ptr());


	//15
	vPos = { -5.f, 2.4f, 23.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.7f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[18].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[19].emplace_back(pDecoProp->Get_Light_Ptr());


	//16
	vPos = { -9.27f, 2.4f, 29.8f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[18].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[19].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[20].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[22].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[23].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트그룹 제거
	//lightGroup[24].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 그룹추가
	lightGroup[25].emplace_back(pDecoProp->Get_Light_Ptr());



	//17
	vPos = { -5.7f, 2.4f, 31.5f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[19].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[20].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[22].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[23].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트그룹 제거
	//lightGroup[24].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 그룹추가
	lightGroup[25].emplace_back(pDecoProp->Get_Light_Ptr());


	//18
	vPos = { -8.5f, 2.4f, 37.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[21].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[23].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[24].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[25].emplace_back(pDecoProp->Get_Light_Ptr());


	//19
	vPos = { 1.f, 2.4f, 32.2f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[20].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[21].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[22].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[25].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[26].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[27].emplace_back(pDecoProp->Get_Light_Ptr());


	//20
	vPos = { -0.8f, 2.4f, 36.7f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);

	lightGroup[20].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[21].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[22].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[23].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[24].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[25].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[26].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[27].emplace_back(pDecoProp->Get_Light_Ptr());

	//21
	vPos = { -2.5f, 2.4f, 44.4f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[25].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[26].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[27].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[28].emplace_back(pDecoProp->Get_Light_Ptr());

	//22
	vPos = { -15.f, 2.4f, 44.2f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[27].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[28].emplace_back(pDecoProp->Get_Light_Ptr());

	//23
	vPos = { -24.7f, 2.4f, 44.f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[27].emplace_back(pDecoProp->Get_Light_Ptr());
	lightGroup[28].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 29번 토글 없음	 
	//lightGroup[29].emplace_back(pDecoProp->Get_Light_Ptr());

	//24
	vPos = { -27.f, 2.4f, 39.2f };
	pGameObject = pDecoProp = CDecorativeProp::Create(m_pGraphicDev, DECO_LAMP, vPos, 3.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject), E_FAIL);
	lightGroup[28].emplace_back(pDecoProp->Get_Light_Ptr());
	//TODO:라이트 29번 토글 없음	 
	//lightGroup[29].emplace_back(pDecoProp->Get_Light_Ptr());

	//LightToggle

	vPos = { -13.f,2.f,-11.f };
	lowerBound = vPos - D3DXVECTOR3{1.f,1.f,1.f};
	upperBound = vPos + D3DXVECTOR3{1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[0],vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -18.5f,2.f,-11.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[1], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -10.f,2.f,-7.f };
	lowerBound = vPos - D3DXVECTOR3{ 2.f,1.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 2.f,1.f,2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[2], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -13.f,2.f,-4.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[3], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -20.5f, 2.f,-2.5f };
	lowerBound = vPos - D3DXVECTOR3{ 1.5f,1.f,2.5f };
	upperBound = vPos + D3DXVECTOR3{ 1.5f,1.f,2.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[4], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -23.f, 2.f,-5.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[5], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	//TODO:토글위치수정
	vPos = { -13.f, 2.f,-1.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[6], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	//TODO:라이트 토글 위치 크기 수정
	vPos = { -10.f, 2.f,2.5f };
	lowerBound = vPos - D3DXVECTOR3{ 2.5f,1.f,0.25f };
	upperBound = vPos + D3DXVECTOR3{ 2.5f,1.f,0.25f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[7], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -2.f, 2.f,-1.f };
	lowerBound = vPos - D3DXVECTOR3{ 2.f,1.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 2.f,1.f,2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[8], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);


	Engine::Clear_Light(m_pGraphicDev);

	for (int i = 0; i < lightGroup[8].size(); ++i)
	{
		Engine::Enable_Light(m_pGraphicDev,
			lightGroup[8][i]->Get_LightID(), -1,
			true,
			lightGroup[8][i]->Get_Light_Pos(),
			lightGroup[8][i]->Get_Light_Range()
		);
	}

	vPos = { 3.f, 2.f,3.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[9], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	//TODO:라이트 토글 위치크기 수정
	vPos = { -8.f, 2.f,0.f };
	lowerBound = vPos - D3DXVECTOR3{ 0.25f,1.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 0.25f,1.f,2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[10], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -16.f, 2.f,4.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[11], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);
	//TODO:라이트 토글 수정 크기위치
	vPos = { -9.5f, 2.f,6.f };
	lowerBound = vPos - D3DXVECTOR3{ 2.f,1.f,0.5f };
	upperBound = vPos + D3DXVECTOR3{ 2.f,1.f,0.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[12], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -7.f, 2.f,7.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[13], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { 3.f, 2.f,5.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[15], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -18.f, 2.f,8.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[16], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -18.f, 2.f,8.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,2.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[16], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	//vPos = { -12.5f, 2.f,8.f };
	//lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	//upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	//pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[17], vPos, lowerBound, upperBound);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -9.f, 2.f, 17.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f,1.f,1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f,1.f,1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[18], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -11.f, 2.f, 22.5f };
	lowerBound = vPos - D3DXVECTOR3{ 2.f,1.f,1.5f };
	upperBound = vPos + D3DXVECTOR3{ 2.f,1.f,1.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[19], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -5.f, 2.f, 28.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f, 1.f, 1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f, 1.f, 1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[20], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { 1.f, 2.f, 29.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f, 1.f, 1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f, 1.f, 1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[21], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { 1.f, 2.f, 33.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f, 1.f, 1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f, 1.f, 1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[22], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -12.f, 2.f, 33.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f, 1.f, 1.f };
	upperBound = vPos + D3DXVECTOR3{ 1.f, 1.f, 1.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[23], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);


	vPos = { -9.f, 2.f, 36.5f };
	lowerBound = vPos - D3DXVECTOR3{ 1.5f, 1.f, 1.5f };
	upperBound = vPos + D3DXVECTOR3{ 1.5f, 1.f, 1.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[24], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -12.5f, 2.f, 36.5f };
	lowerBound = vPos - D3DXVECTOR3{ 1.5f, 1.f, 1.5f };
	upperBound = vPos + D3DXVECTOR3{ 1.5f, 1.f, 1.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[25], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	vPos = { -1.5f, 2.f, 40.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.5f, 1.f, 2.f };
	upperBound = vPos + D3DXVECTOR3{ 1.5f, 1.f, 2.f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[26], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	//TODO:라이트 27번 토글
	vPos = { -2.f, 2.f, 42.f };
	lowerBound = vPos - D3DXVECTOR3{ 1.f, 1.f, 0.5f };
	upperBound = vPos + D3DXVECTOR3{ 1.f, 1.f, 0.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[27], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	//TODO:라이트 28번 토글
	vPos = { -21.f, 2.f, 44.f };
	lowerBound = vPos - D3DXVECTOR3{ 0.5f, 1.f, 0.5f };
	upperBound = vPos + D3DXVECTOR3{ 0.5f, 1.f, 0.5f };
	pGameObject = CLightToggle::Create(m_pGraphicDev, lightGroup[28], vPos, lowerBound, upperBound);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightToggle", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(wstring pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	// 나중에 전부 4/3 곱할것 ( 960, 540 )
	pGameObject = CHUDUICamera::Create(m_pGraphicDev, 270.f, 180.f, 135.f, 450.f, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UILeft", pGameObject), E_FAIL);

	pGameObject = CHUDUICamera::Create(m_pGraphicDev, 270.f, 180.f, 825.f, 450.f, 2);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UIRight", pGameObject), E_FAIL);

	pGameObject = CHUDUICamera::Create(m_pGraphicDev, 56.1f, 55.f, 916.f, 460.f, 13);
	NULL_CHECK_RETURN(pGameObject, E_FAIL)
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UIMagic", pGameObject), E_FAIL);

	pGameObject = CBarUICamera::Create(m_pGraphicDev, 128.f, 18.f, 157.f, 495.f, m_pPlayer, BAR_MANA, 3);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UIMana", pGameObject), E_FAIL);

	pGameObject = CBarUICamera::Create(m_pGraphicDev, 128.f, 18.f, 185.f, 519.f, m_pPlayer, BAR_HEALTH, 4);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UIHealth", pGameObject), E_FAIL);

	pGameObject = CBarUICamera::Create(m_pGraphicDev, 225.f, 28.f, 832.f, 510.f, m_pPlayer, BAR_BULLET, 8);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UIBullet", pGameObject), E_FAIL);

	pGameObject = CPlayerUICamera::Create(m_pGraphicDev, 600.f, 300.f, 480.f, 390.f, m_pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerUICamera", pGameObject), E_FAIL);

	pGameObject = CFaceUICamera::Create(m_pGraphicDev, 73.f, 55.f, 52.f, 499.f, m_pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FaceUICamera", pGameObject), E_FAIL);

	pGameObject = CKeyUICamera::Create(m_pGraphicDev, 34.f, 14.f, 33.f, 434.f, m_pPlayer, DOOR_RED);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"KeyUICamera", pGameObject), E_FAIL);

	pGameObject = CKeyUICamera::Create(m_pGraphicDev, 34.f, 14.f, 33.f, 448.f, m_pPlayer, DOOR_YELLOW);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"KeyUICamera", pGameObject), E_FAIL);

	pGameObject = CKeyUICamera::Create(m_pGraphicDev, 34.f, 14.f, 33.f, 462.f, m_pPlayer, DOOR_BLUE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"KeyUICamera", pGameObject), E_FAIL);

	CShopUICamera* pShopUI;
	pGameObject = pShopUI = CShopUICamera::Create(m_pGraphicDev, 678.f, 406.8, 480.f, 270.f, m_pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShopUICamera", pGameObject), E_FAIL);

	pGameObject = CCursor::Create(m_pGraphicDev, pShopUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cursor", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_LightInfo()
{
	D3DLIGHT9* tLightInfo = new D3DLIGHT9;
	m_lLight.push_back(tLightInfo);
	ZeroMemory(tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo->Diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	tLightInfo->Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	tLightInfo->Specular = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	tLightInfo->Direction = D3DVECTOR({ 0.f, -1.f, -1.f });
	Engine::Ready_Light(m_pGraphicDev, m_lLight.back(), LIGHT_DEFAULT);

	//조명
	D3DLIGHT9* tPurplePoint = new D3DLIGHT9;
	m_lLight.push_back(tPurplePoint);
	ZeroMemory(tPurplePoint, sizeof(D3DLIGHT9));

	tPurplePoint->Type = D3DLIGHT_POINT;

	tPurplePoint->Diffuse = D3DXCOLOR(0.2f, 0.f, 0.4f, 1.f);
	tPurplePoint->Ambient = D3DXCOLOR(0.2f, 0.f, 0.4f, 1.f);
	tPurplePoint->Specular = D3DXCOLOR(0.2f, 0.f, 0.4f, 1.f);
	tPurplePoint->Attenuation0 = 1.f;
	tPurplePoint->Range = 3.f;
	Engine::Ready_Light(m_pGraphicDev, m_lLight.back(), LIGHT_REACTOR);

	//조명
	D3DLIGHT9* tWhitePoint = new D3DLIGHT9;
	m_lLight.push_back(tWhitePoint);
	ZeroMemory(tWhitePoint, sizeof(D3DLIGHT9));

	tWhitePoint->Type = D3DLIGHT_POINT;

	tWhitePoint->Diffuse = D3DXCOLOR(1.f, 0.7f, 0.f, 1.f);
	tWhitePoint->Ambient = D3DXCOLOR(1.f, 0.7f, 0.f, 1.f);
	tWhitePoint->Specular = D3DXCOLOR(1.f, 0.7f, 0.f, 1.f);
	tWhitePoint->Attenuation0 = 1.f;
	tWhitePoint->Range = 3.f;
	Engine::Ready_Light(m_pGraphicDev, m_lLight.back(), LIGHT_ROCKET);

	//조명
	D3DLIGHT9* tLampPoint = new D3DLIGHT9;
	m_lLight.push_back(tLampPoint);
	ZeroMemory(tLampPoint, sizeof(D3DLIGHT9));

	tLampPoint->Type = D3DLIGHT_POINT;

	tLampPoint->Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLampPoint->Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLampPoint->Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLampPoint->Attenuation0 = 1.f;
	tLampPoint->Range = 3.f;
	Engine::Ready_Light(m_pGraphicDev, m_lLight.back(), LIGHT_LAMP);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	Engine::Enable_Light(m_pGraphicDev);

	//사이렌조명
	D3DLIGHT9* tLightSpot = new D3DLIGHT9;
	m_lLight.push_back(tLightSpot);
	ZeroMemory(tLightSpot, sizeof(D3DLIGHT9));

	tLightSpot->Type = D3DLIGHT_SPOT;

	tLightSpot->Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	tLightSpot->Ambient = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);
	tLightSpot->Specular = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);
	tLightSpot->Attenuation0 = 0.0001f;
	tLightSpot->Attenuation1 = 0.0001f;
	tLightSpot->Attenuation2 = 0.0001f;
	tLightSpot->Falloff = 1.0f;
	tLightSpot->Range = 4.5f;
	//tLightSpot->Direction = _vec3(1, -1, 1);
	tLightSpot->Theta = D3DX_PI / 3;//내부각
	tLightSpot->Phi = D3DX_PI / 1;//외부각
	Engine::Ready_Light(m_pGraphicDev, m_lLight.back(), LIGHT_SPOT);

	// 무기 총쏠때 조명
	D3DLIGHT9* tLightShoot = new D3DLIGHT9;
	m_lLight.push_back(tLightShoot);
	ZeroMemory(tLightShoot, sizeof(D3DLIGHT9));

	tLightShoot->Type = D3DLIGHT_POINT;
	tLightShoot->Diffuse = D3DXCOLOR(0.6f, 0.3f, 0.0f, 1.f);
	tLightShoot->Ambient = D3DXCOLOR(0.6f, 0.3f, 0.0f, 1.f);
	tLightShoot->Specular = D3DXCOLOR(0.6f, 0.3f, 0.0f, 1.f);
	tLightShoot->Attenuation0 = 1.f;
	tLightShoot->Range = 3.f;
	Engine::Ready_Light(m_pGraphicDev, m_lLight.back(), LIGHT_SHOOT);

	//재질
	D3DMATERIAL9* white = new D3DMATERIAL9;
	m_lMaterial.push_back(white);
	ZeroMemory(white, sizeof(D3DMATERIAL9));
	white->Diffuse = D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f };
	white->Ambient = D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f };
	white->Specular = D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f };
	white->Emissive = D3DXCOLOR{ 0.f, 0.f, 0.f, 1.f };
	Engine::Ready_Material(m_lMaterial.back());

	//재질
	D3DMATERIAL9* siren = new D3DMATERIAL9;
	m_lMaterial.push_back(siren);
	ZeroMemory(siren, sizeof(D3DMATERIAL9));
	siren->Diffuse = D3DXCOLOR{ 1.f, 0.f, 0.f, 1.f };
	siren->Ambient = D3DXCOLOR{ 1.f, 0.f, 0.f, 1.f };
	siren->Specular = D3DXCOLOR{ 1.f, 0.f, 0.f, 1.f };
	siren->Emissive = D3DXCOLOR{ 1.f, 0.f, 0.f, 0.6f };
	Engine::Ready_Material(m_lMaterial.back(), MTRL_SIREN);

	//재질
	D3DMATERIAL9* reactor = new D3DMATERIAL9;
	m_lMaterial.push_back(reactor);
	ZeroMemory(reactor, sizeof(D3DMATERIAL9));
	reactor->Diffuse = D3DXCOLOR{ 0.f, 0.f, 0.f, 1.f };
	reactor->Ambient = D3DXCOLOR{ 0.f, 0.f, 0.f, 1.f };
	reactor->Specular = D3DXCOLOR{ 0.f, 0.f, 0.f, 1.f };
	reactor->Emissive = D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f };
	Engine::Ready_Material(m_lMaterial.back(), MTRL_REACTOR);

	Engine::Enable_Material(m_pGraphicDev);



	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CStage::Free()
{
	__super::Free();
	Engine::Clear_Light(m_pGraphicDev);
}
