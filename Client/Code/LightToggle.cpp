#include "stdafx.h"
#include "LightToggle.h"
#include "Export_Utility.h"
#include "GameData.h"
#include "LightMgr.h"

CLightToggle::CLightToggle(const CLightToggle& rhs)
	:CGameObject(rhs),m_vecLightIdxGroup(rhs.m_vecLightIdxGroup), m_pTransformCom(rhs.m_pTransformCom)
{
}

CLightToggle::CLightToggle(LPDIRECT3DDEVICE9 pGraphicDev, vector<CLight*> vecLightIdxGroup)
	: CGameObject(pGraphicDev), m_vecLightIdxGroup(vecLightIdxGroup), m_pTransformCom(nullptr)
{
}

CLightToggle::~CLightToggle()
{
}

HRESULT CLightToggle::Ready_GameObject(D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(vPos);
	m_vPrePos = m_pTransformCom->Get_Pos();
	Push_CollisionMgr(OT_LIGHTTOGGLE, lowerBound, upperBound);

	m_bColliderIsNone = false;

	return S_OK;
}

_int CLightToggle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	int iExit = CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CLightToggle::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);

	m_fElipsedTime += fTimeDelta;
}

void CLightToggle::Render_GameObject()
{
}

void CLightToggle::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
	if (pGameObject->Get_ObjType() == OT_PLAYER)
	{
		if (fabs(m_fElipsedTime - fTimeDelta) < 0.0001f)
		{
			m_fElipsedTime = 0.f;
			return;
		}
		Engine::Clear_Light(m_pGraphicDev);

		for (int i = 0; i < m_vecLightIdxGroup.size(); ++i)
		{
			Engine::Enable_Light(m_pGraphicDev,
				m_vecLightIdxGroup[i]->Get_LightID(), -1,
				true,
				m_vecLightIdxGroup[i]->Get_Light_Pos(),
				m_vecLightIdxGroup[i]->Get_Light_Range()
			);
		}

		m_fElipsedTime = 0.f;
	}

	
}

void CLightToggle::Action_On_Collision(vector<CLine*> pvecLines)
{
}

HRESULT CLightToggle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CLightToggle* CLightToggle::Create(LPDIRECT3DDEVICE9 pGraphicDev, vector<CLight*> vecLightIdxGroup, D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	CLightToggle* pInstance = DBG_NEW CLightToggle(pGraphicDev, vecLightIdxGroup);

	if (FAILED(pInstance->Ready_GameObject(vPos, lowerBound, upperBound)))
	{
		Safe_Release(pInstance);
		MSG_BOX("LightToggle Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CLightToggle::Free()
{
	__super::Free();
}
