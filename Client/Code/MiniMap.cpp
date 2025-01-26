#include "stdafx.h"
#include "MiniMap.h"
#include "Export_Utility.h"
#include "Player.h"
#include "GameData.h"
#include "MeshGroup.h"
#include "MiniMapUITex.h"

CMiniMap* CMiniMap::m_pInstance = nullptr;
CMiniMap* CMiniMap::GetInstance(void) {
	if (m_pInstance == nullptr)
	{
		return nullptr;
	}
	return m_pInstance;
}

void CMiniMap::DestroyInstance(void) {

	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_MiniMapVP{}
{
	m_pGraphicDev->AddRef();
}

CMiniMap::~CMiniMap()
{
	Free();
}

HRESULT CMiniMap::Add_Component(vector<int> FloorMeshIdx)
{
	m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GrayMiniMapTexture"));


	TCHAR szProtoName[128] = L"Proto_Mesh%d";
	for (int i = 0; i < FloorMeshIdx.size(); i++)
	{
		Engine::CMesh* pMesh = nullptr;

		TCHAR	szName[128] = L"";

		wsprintf(szName, szProtoName, FloorMeshIdx[i]);
		
		pMesh = dynamic_cast<CMesh*>(Engine::Clone_Proto(szName));
		NULL_CHECK_RETURN(pMesh, E_FAIL);
		m_pMeshGroupCom.push_back(pMesh);
	}
	

	m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);



	return S_OK;
}

void CMiniMap::UpdateCamera(const _float& fTimeDelta)
{
}

void CMiniMap::Free()
{
	Safe_Release(m_pMiniMapSurface);
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pCamera);
	Safe_Release(m_pUITex);

	for (int i = 0; i < m_pMeshGroupCom.size(); ++i)
	{
		Safe_Release(m_pMeshGroupCom[i]);
	}

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}

HRESULT CMiniMap::Ready_MiniMap(CPlayer* pPlayer, vector<int> FloorMeshIdx)
{
	m_pPlayer = pPlayer;
	FAILED_CHECK_RETURN(Add_Component(FloorMeshIdx), E_FAIL);


	const int miniMapWidth = 200;
	const int miniMapHeight = 200;

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateRenderTarget(
		miniMapWidth,
		miniMapHeight,
		D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,   // 멀티샘플링 유형
		0, // 멀티샘플링 품질
		FALSE,  // 잠금 가능 여부
		&m_pMiniMapSurface,
		NULL
	), E_FAIL);


	m_MiniMapVP.X = 0.f;
	m_MiniMapVP.Y = 0.f;
	m_MiniMapVP.Width = 200;
	m_MiniMapVP.Height = 200;
	m_MiniMapVP.MinZ = 0.0f;  
	m_MiniMapVP.MaxZ = 1.0f;

	D3DXVECTOR3 vPos = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() + D3DXVECTOR3(0.f, 5.f, 0.f);
	D3DXVECTOR3 vAt = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() + D3DXVECTOR3{0.f,-1.f,0.f};
	D3DXVECTOR3 vUp;
	m_pPlayer->Get_TransformCom_Pointer()->Get_Info(INFO_LOOK, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);
	float fFov = m_pPlayer->Get_Player_Fov();
	float fAspect = m_pPlayer->Get_Player_Aspect();
	float fNear = m_pPlayer->Get_Player_Near();
	float fFar = m_pPlayer->Get_Player_Far();
	

	m_pCamera = CCamera::Create(m_pGraphicDev, vPos, vAt, vUp, fFov, fAspect, fNear, fFar);
	m_pUITex = CMiniMapUITex::Create(m_pGraphicDev, L"Proto_YellowDotTexture", 30 * WINCX / WINCY, 30 , WINCX / 2, WINCY/2, 0.f);
	return S_OK;
}

_int CMiniMap::Update_MiniMap(const _float& fTimeDelta)
{
	m_pCamera->Update_GameObject(fTimeDelta);
	m_pUITex->Update_GameObject(fTimeDelta);
	return 0;
}

void CMiniMap::LateUpdate_MiniMap(const float& fTimeDelta)
{
	D3DXVECTOR3 vPos = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() + D3DXVECTOR3(0.f,5.f,0.f);
	D3DXVECTOR3 vAt = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() + D3DXVECTOR3{ 0.f,-1.f,0.f };
	D3DXVECTOR3 vUp;
	m_pPlayer->Get_TransformCom_Pointer()->Get_Info(INFO_LOOK, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pCamera->Set_Eye(vPos);
	m_pCamera->Set_At(vAt);
	m_pCamera->Set_Up(vUp);

	m_pCamera->LateUpdate_GameObject(fTimeDelta);
	m_pUITex->LateUpdate_GameObject(fTimeDelta);

}

void CMiniMap::Render_MiniMap()
{
	m_pGraphicDev->SetRenderTarget(1, m_pMiniMapSurface);
	m_pGraphicDev->SetViewport(&m_MiniMapVP);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pCamera->Set_View_Space();

	m_pTextureCom->Set_Texture(0);

	for (int i = 0; i < m_pMeshGroupCom.size(); i++)
	{
		m_pMeshGroupCom[i]->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xf0);
	m_pUITex->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pPlayer->Set_View_Space();
	m_pPlayer->Set_Projection_Space();

}

HRESULT CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer, vector<int> FloorMeshIdx)
{
	if (m_pInstance != nullptr)
	{
		MSG_BOX("MiniMap is Already Created");
		return E_FAIL;
	}

	m_pInstance = DBG_NEW CMiniMap(pGraphicDev);

	if (FAILED(m_pInstance->Ready_MiniMap(pPlayer, FloorMeshIdx)))
	{
		Safe_Release(m_pInstance);
		MSG_BOX(" Create Failed");
		return E_FAIL;
	}
	return S_OK;
}
