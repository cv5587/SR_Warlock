#include "stdafx.h"
#include "..\Header\MainApp.h"
#include "Logo.h"
#include "Stage.h"
#include "GameData.h"
#include "Player.h"
#include "MiniMap.h"


CMainApp::CMainApp() : m_pDeviceClass(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_Setting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	srand(unsigned(time(NULL)));

	return S_OK;
}

int CMainApp::Update_MainApp(const float & fTimeDelta)
{
	Engine::Update_InputDev();

	long dwMouse(0);

	POINT pt;
	GetCursorPos(&pt); // 현재 마우스 위치를 얻습니다.
	ScreenToClient(g_hWnd, &pt);

	// 마우스 위치를 윈도우의 크기 내로 제한합니다.
	if (pt.x > WINCX)
	{
		pt.x = WINCX;
	}
	else if (0 > pt.x)
	{
		pt.x = 0;
	}

	if (pt.y > WINCY)
	{
		pt.y = WINCY;
	}
	else if ( 0 > pt.y)
	{
		pt.y = 0;
	}

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y); // 마우스 위치를 설정합니다.
	
	m_pManagementClass->Update_Scene(fTimeDelta);

	if (CMiniMap::GetInstance() != nullptr)
	{
		CMiniMap::GetInstance()->Update_MiniMap(fTimeDelta);
	}

	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
	m_pManagementClass->LateUpdate_Scene(fTimeDelta);

	if (CMiniMap::GetInstance() != nullptr)
	{
		CMiniMap::GetInstance()->LateUpdate_MiniMap(fTimeDelta);
	}
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.2f, 0.094f, 0.f, 1.f));

	m_pGraphicDev->SetRenderTarget(0, m_pOriginalSurface);
	m_pGraphicDev->SetViewport(&m_MainVP);

	m_pManagementClass->Render_Scene(m_pGraphicDev);	

	//for_debug

	if (Engine::Get_GameObject(L"GameLogic", L"Player") != nullptr)
	{
		if (static_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Is_Collider_Visible())
		{
			Engine::Render_Colliders();
		}

	}

	if (CMiniMap::GetInstance() != nullptr)
	{
		CMiniMap::GetInstance()->Render_MiniMap();
	}

	m_pGraphicDev->SetRenderTarget(0, m_pOriginalSurface);

	Engine::Render_End();

}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagement)
{
	Engine::CScene*		pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);
	
	return S_OK;
}

HRESULT CMainApp::SetUp_Setting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	//(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		  // Z버퍼에 깊이 값을 기록은 하지만 자동 정렬을 수행할지 말지 결정
	//(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 픽셀의 깊이 값을 저장할지 말지 결정
	
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕체", 30, 30, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Bullet", L"바탕체", 15, 30, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Health", L"바탕체", 10, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Soo", L"바탕체", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Menu", L"바탕체", 10, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Name", L"바탕체", 10, 12, FW_HEAVY), E_FAIL);
	Engine::Ready_SoundMgr();
	
	m_pGraphicDev->GetRenderTarget(0, &m_pOriginalSurface);

	// Dinput
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);


	m_MainVP.X = 0;
	m_MainVP.Y = 0;
	m_MainVP.Width = WINCX; // 미니맵의 너비
	m_MainVP.Height = WINCY; // 미니맵의 높이
	m_MainVP.MinZ = 0.0f;
	m_MainVP.MaxZ = 1.0f;

	
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp *		pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
	Engine::Safe_Release(m_pManagementClass);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pGraphicDev);

	CGameData::DestroyInstance();
	CMiniMap::DestroyInstance();
	Engine::Release_Utility();
	Engine::Release_System();
}