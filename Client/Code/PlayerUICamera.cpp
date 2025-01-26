#include "stdafx.h"
#include "..\Header\PlayerUICamera.h"

#include "Export_System.h"
#include "stdio.h"

CPlayerUICamera::CPlayerUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer)
	:CUICamera(pGraphicDev), m_pPlayer(pPlayer)
{
}

CPlayerUICamera::CPlayerUICamera(const CPlayerUICamera& rhs)
	:CUICamera(rhs)
{
}

CPlayerUICamera::~CPlayerUICamera()
{
}

HRESULT CPlayerUICamera::Ready_GameObject(const _float& fWidth,	const _float& fHeight, const _float& fPosX, const _float& fPosY)
{
	Engine::CUICamera::Ready_UICamera();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 맨 위에 출력하기 위해 값 설정
	m_iSortOrder = 999;
	m_pTransformCom->m_vScale.x = fWidth * 0.5f;
	m_pTransformCom->m_vScale.y = fHeight * 0.5f;
	m_fUIRadius = 50.f;
	m_vCentralPos = _vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f - m_fUIRadius, 0.0f);
	m_eUIDir = UIDIR_RIGHT;
	m_fUIAngle = D3DX_PI / 2;
	m_fSwingSpeed = 2.f;
	return S_OK;
}

_int CPlayerUICamera::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CPlayerUICamera::LateUpdate_GameObject(const float& fTimeDelta)
{
	if (m_pPlayer->Is_Moving() && !m_pPlayer->Get_Weapon()->Get_Attack())
	{
		switch (m_eUIDir)
		{
		case UIDIR_RIGHT:
			m_fUIAngle -= fTimeDelta * m_fSwingSpeed * D3DX_PI;
			if (m_fUIAngle <= 0)
			{
				m_fUIAngle = 0;
				m_eUIDir = UIDIR_LEFT;
			}
			break;
		case UIDIR_LEFT:
			m_fUIAngle += fTimeDelta * m_fSwingSpeed * D3DX_PI;
			if (m_fUIAngle >= D3DX_PI)
			{
				m_fUIAngle = D3DX_PI;
				m_eUIDir = UIDIR_RIGHT;
			}
		}
	}


	if (m_pPlayer->Get_Cur_Weapon() == STAFF)
	{
		m_pTransformCom->Set_Pos(m_vCentralPos.x + m_fUIRadius * cosf(m_fUIAngle), m_vCentralPos.y - 50.f + m_fUIRadius * sinf(m_fUIAngle), m_vCentralPos.z);
	}
	else
	{
		m_pTransformCom->Set_Pos(m_vCentralPos.x + m_fUIRadius * cosf(m_fUIAngle), m_vCentralPos.y + m_fUIRadius * sinf(m_fUIAngle), m_vCentralPos.z);
	}

	Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CPlayerUICamera::Render_GameObject()
{
	Engine::CUICamera::Render_GameObject();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pPlayer->Get_Weapon()->Render_GameObject();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_pPlayer->Get_Nearby())
	{
		Engine::Render_Font(L"Font_Bullet", L"Press SPACE to interact.", &_vec2(300.f, 100.f), D3DXCOLOR(0.51f, 0.51f, 0.51f, 0.8f));		
	}
}

HRESULT CPlayerUICamera::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CPlayerUICamera* CPlayerUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fWidth, const _float& fHeight, const _float& fPosX, const _float& fPosY, CPlayer*& pPlayer)
{
	CPlayerUICamera* pInstance = DBG_NEW CPlayerUICamera(pGraphicDev, pPlayer);

	if (FAILED(pInstance->Ready_GameObject(fWidth, fHeight, fPosX, fPosY)))
	{
		Safe_Release(pInstance);
		MSG_BOX("GameUICamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayerUICamera::Free()
{
	Engine::CUICamera::Free();
}