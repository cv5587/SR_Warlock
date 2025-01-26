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

#include "ShopUICamera.h"
#include "Cursor.h"
#include "GameData.h"

#include "MainMenu.h"
#include "ShopNPC.h"
#include "MiniMap.h"

CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev), m_pPlayer(nullptr), m_teleport(false)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene(CPlayer& player)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic", player), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	

	Engine::Get_SoundMgr()->Play_BGM(L"BGM_BossStage.wav", 0.25f);

	return S_OK;
}

Engine::_int CBossStage::Update_Scene(const _float& fTimeDelta)
{	
	m_pPlayer->Set_Nearby(false);
	//teleport 코드
	//if (m_pExit->Is_Teleport_Pos())
	//{
	//	m_teleport = true;
	//	Engine::CScene* pScene = nullptr;
	//	////TODO
	//	//pScene = CMainMenu::Create(m_pGraphicDev);
	//	//NULL_CHECK_RETURN(pScene, -1);

	//	//FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
	//	return 0;
	//}

	return __super::Update_Scene(fTimeDelta);
}

void CBossStage::LateUpdate_Scene(const float& fTimeDelta)
{
	__super::LateUpdate_Scene(fTimeDelta);

	Engine::LateUpdate_CollisionMgr(fTimeDelta);


		
}

void CBossStage::Render_Scene()
{

	
	if (m_pPlayer)
	{
		const _tchar* szProtoName = L"Player Pos: (X : %.2f, Y : %.2f, Z: %.2f) ";		
		const _vec3 vPlayerPos = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

		TCHAR	szPlayerPos[128] = L"";

		swprintf_s(szPlayerPos, szProtoName, vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);

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
	}


	//Engine::DrawMesh();
	// DEBUG
}

HRESULT CBossStage::Ready_Layer_Environment(wstring pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);
		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_GameLogic(wstring pLayerTag, CPlayer& player)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = m_pPlayer = CPlayer::Clone_Create(player,
		//y = 2,1f
		D3DXVECTOR3(0.f, 2.1f, -1.f), D3DXVECTOR3(0.f, D3DXToRadian(180.f), 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	Engine::Set_Camera(static_cast<Engine::CCamera*>(pGameObject));

	const multimap<std::wstring, CGameObject*>& tmpObjGroup = CGameData::GetInstance()->Get_TotalGroup(1);

	for (auto& iter : tmpObjGroup)
	{
		iter.second->Push_CollisionMgr();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(iter.first, iter.second ), E_FAIL);
	}
	CGameData::GetInstance()->Set_IsCloneTotalGroups(1);

	CMeshGroup* tmpMeshGroup = CGameData::GetInstance()->Get_MeshGroup(1);
	tmpMeshGroup->Set_Is_Clone(true);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MeshGroup", tmpMeshGroup));
	
	CGameData::GetInstance()->Activate_LineCollision(1);

	FAILED_CHECK_RETURN(CMiniMap::Create(m_pGraphicDev, m_pPlayer, CGameData::GetInstance()->Get_FloorMeshIndices(1)), E_FAIL)

	pGameObject = CSnow::Create(m_pGraphicDev, _vec3(-4, 1, 30), 0.1f, 1024, 10.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Snow", pGameObject), E_FAIL);

	//pGameObject = CSnow::Create(m_pGraphicDev, _vec3(-6, 1, 30), 0.1f, 1024, 10.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Snow", pGameObject), E_FAIL);

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

	//pGameObject = CExit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Exit", pGameObject), E_FAIL);

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

	pGameObject = CShambler::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shambler", pGameObject), E_FAIL);

	pGameObject = CBoss_Phase1::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss1", pGameObject), E_FAIL);


	pGameObject = CBoss_Phase2::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss2", pGameObject), E_FAIL);


	//for (_int i = 0; i < 50; ++i)
	//{
	//	pGameObject = CEffect::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
	//}
	
	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CBossStage::Ready_Layer_UI(wstring pLayerTag)
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

	pGameObject = CPlayerUICamera::Create(m_pGraphicDev, 480.f, 240.f, 480.f, 420.f, m_pPlayer);
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

HRESULT CBossStage::Ready_LightInfo()
{
	D3DLIGHT9* tLightInfo = new D3DLIGHT9;
	m_lLight.push_back(tLightInfo);
	ZeroMemory(tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo->Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo->Diffuse = D3DXCOLOR(0.45f, 0.5f, 0.55f, 1.f);
	tLightInfo->Ambient = D3DXCOLOR(0.45f, 0.5f, 0.55f, 1.f);
	tLightInfo->Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
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

	tLampPoint->Diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	tLampPoint->Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	tLampPoint->Specular = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
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

CBossStage * CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer& player)
{
	CBossStage *	pInstance = new CBossStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(player)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CBossStage::Free()
{
	__super::Free();
}
