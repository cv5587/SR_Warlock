#include "stdafx.h"
#include "MainMenu.h"
#include "Export_System.h"
#include "Stage.h"
#include "Boss_Stage.h"

CMainMenu::CMainMenu(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev), m_pLoading(nullptr), m_bGameStartBtn(false), m_bExitBtn(false)
{
}

CMainMenu::~CMainMenu()
{
}

HRESULT CMainMenu::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	ShowCursor(true);

	Engine::Get_SoundMgr()->StopAll();
	Engine::Get_SoundMgr()->Play_BGM(L"BGM_MainMenu.wav", 0.5f);

	return S_OK;
}

_int CMainMenu::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);

	POINT pt;
	GetCursorPos(&pt); // 현재 마우스 위치를 얻습니다.
	ScreenToClient(g_hWnd, &pt);

	if (pt.x <= (m_UIGameStart->Get_PosX() + m_UIGameStart->Get_SizeX() * 0.5f) &&
		pt.x >= (m_UIGameStart->Get_PosX() - m_UIGameStart->Get_SizeX() * 0.5f) &&
		pt.y <= (m_UIGameStart->Get_PosY() + m_UIGameStart->Get_SizeY() * 0.5f) &&
		pt.y >= (m_UIGameStart->Get_PosY() - m_UIGameStart->Get_SizeY() * 0.5f))
		m_bGameStartBtn = true;
	else
		m_bGameStartBtn = false;

	if (pt.x <= (m_UIExit->Get_PosX() + m_UIExit->Get_SizeX() * 0.5f) &&
		pt.x >= (m_UIExit->Get_PosX() - m_UIExit->Get_SizeX() * 0.5f) &&
		pt.y <= (m_UIExit->Get_PosY() + m_UIExit->Get_SizeY() * 0.5f) &&
		pt.y >= (m_UIExit->Get_PosY() - m_UIExit->Get_SizeY() * 0.5f))
		m_bExitBtn = true;
	else
		m_bExitBtn = false;


		if (m_bGameStartBtn&& Get_DIMouseState(DIM_LB) & 0x80 && !(Get_DIMouseState(DIM_LB) & 0x40))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
			return 0;
		}

		if (m_bExitBtn && Get_DIMouseState(DIM_LB) & 0x80 && !(Get_DIMouseState(DIM_LB) & 0x40))
		{
			DestroyWindow(g_hWnd);
			return 0;
		}

	return iExit;
}

void CMainMenu::LateUpdate_Scene(const float& fTimeDelta)
{
	__super::LateUpdate_Scene(fTimeDelta);
}

void CMainMenu::Render_Scene()
{
	if (m_bGameStartBtn)
		Render_Font(L"Font_Menu", L"GAME START", &_vec2(m_UIGameStart->Get_PosX() - 65, m_UIGameStart->Get_PosY() - 10), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	else
		Render_Font(L"Font_Menu", L"GAME START", &_vec2(m_UIGameStart->Get_PosX() - 65, m_UIGameStart->Get_PosY() - 10), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	
	if (m_bExitBtn)
		Render_Font(L"Font_Menu", L"EXIT", &_vec2(m_UIExit->Get_PosX() - 20, m_UIExit->Get_PosY() - 10), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	else
		Render_Font(L"Font_Menu", L"EXIT", &_vec2(m_UIExit->Get_PosX() - 20, m_UIExit->Get_PosY() - 10), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CMainMenu::Ready_Prototype()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol1x1", CRcCol::Create(m_pGraphicDev,1.f,1.f,D3DXCOLOR(0.f,0.f,0.f,1.f))), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex1x1", CRcTex::Create(m_pGraphicDev,1.f,1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicRcTex1x1", CDynamicRcTex::Create(m_pGraphicDev,true,1.f,1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DynamicRcTex1x1_left", CDynamicRcTex::Create(m_pGraphicDev,false,1.f,1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MenuTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Main_Menu/menu_potwory_v0%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LightingTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Main_Menu/menu_blyskawica_v00.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Black", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Main_Menu/Black.jpg", 1)), E_FAIL);
	return S_OK;
}

HRESULT CMainMenu::Ready_Layer_Environment(wstring pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = m_UIGameStart = CUI::Create(m_pGraphicDev,200, 40, 810, 230, 0.1, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_GAMESTART", pGameObject), E_FAIL);

	pGameObject = m_UIExit = CUI::Create(m_pGraphicDev, 200, 40, 810, 300, 0.1, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_EXIT", pGameObject), E_FAIL);

	pGameObject = CUITex::Create(m_pGraphicDev, L"Proto_Black",300, 540, 810, 270, 0.78, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Banner", pGameObject), E_FAIL);

	pGameObject = m_UITexImage1 = CUITex::Create(m_pGraphicDev, L"Proto_MenuTexture",800, 800, 330, 270, 0.3f, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_UITexImage1->Set_TexIdx(2);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Image1", pGameObject), E_FAIL);

	pGameObject = m_UIDynamicTexImage1 = CUIDynamicTex::Create(m_pGraphicDev, L"Proto_MenuTexture", 660, 360, 330, 380, 0.25f, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_UIDynamicTexImage1->Set_TexIdx(1);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Dynamic_Image1", pGameObject), E_FAIL);

	pGameObject = m_UIDynamicTexImage2 = CUIDynamicTex::Create(m_pGraphicDev, L"Proto_MenuTexture", 660, 360, 330, 230, 0.75f, false ,true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_UIDynamicTexImage2->Set_TexIdx(0);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Dynamic_Image2", pGameObject), E_FAIL);

	pGameObject = m_UIDynamicTexImage3 = CUIDynamicTex::Create(m_pGraphicDev, L"Proto_LightingTexture", 960, 540, 480, 170, 0.8, false,true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_UIDynamicTexImage3->Set_TexIdx(0);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Dynamic_Image3", pGameObject), E_FAIL);



	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CMainMenu* CMainMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainMenu* pInstance = new CMainMenu(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MainMenu Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMainMenu::Free()
{
	__super::Free();
	ShowCursor(false);
}
