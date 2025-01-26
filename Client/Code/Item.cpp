#include "stdafx.h"
#include "Item.h"

#include "Export_Utility.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, ITEMTYPE eItemType, _int iNum)
    : Engine::CGameObject(pGraphicDev), m_eItemtype(eItemType), m_iNum(iNum), m_fFrame(0.f)
{
}

CItem::CItem(const CItem& rhs)
    : Engine::CGameObject(rhs), m_eItemtype(rhs.m_eItemtype), m_iNum(rhs.m_iNum), m_iValue(rhs.m_iValue), m_fFrame(0.f)
{
	Ready_GameObject(rhs.m_pTransformCom->Get_Pos());
	m_pTransformCom->Set_Pos(rhs.m_pTransformCom->Get_Pos());
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject(_vec3 vPos)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransformCom->Set_Scale({ 0.2f, 0.2f, 0.2f });
	m_pTransformCom->Set_Pos(vPos);
	m_vPrePos = m_pTransformCom->Get_Pos();
	m_eObjType = OT_ITEM;

	m_lowerBound = m_vPrePos - D3DXVECTOR3{0.2f, 0.2f, 0.2f };
	m_upperBound = m_vPrePos + D3DXVECTOR3{ 0.2f, 0.2f, 0.2f };

	//CGameObject::Push_CollisionMgr(OT_ITEM, 0.2f);

	switch (m_eItemtype)
	{
	case BULLET:
		if (m_iNum == 3) m_iValue = 100;
		else m_iValue = m_iNum * 5 + 5;
		break;
	case EXP:
		m_iValue = m_iNum * 10 + 10;
		break;
	case HEALTH:
		m_iValue = m_iNum * 20 + 10;
		break;
	case MANA:
		m_iValue = m_iNum * 10 + 5;
		break;
	}

	m_bColliderIsNone = true;

    return S_OK;
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 90.f * fTimeDelta;
	_float m_fDirY = (_float)((_int)m_fFrame % 90 / 45) - 0.5f;
	CGameObject::Move_Pos({ 0.f, m_fDirY, 0.f }, 0.2f, fTimeDelta);

	if (m_bDead) 
		return OBJ_DEAD;

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	CGameObject::Update_GameObject(fTimeDelta);

	Compute_ViewZ(m_pTransformCom->Get_Pos());

	return 0;
}

void CItem::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);
}

void CItem::Render_GameObject()
{

	//// 3축 빌보드
	//_vec3 vPos;
	//_matrix	matView;
	//vPos = m_pTransformCom->Get_Pos();
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixInverse(&matView, NULL, &matView);
	//matView._41 = vPos.x;
	//matView._42 = vPos.y;
	//matView._43 = vPos.z;
	//m_pTransformCom->Set_WorldMatrix(&(matView));

	_matrix	matWorld, matView, matBill, matBillX, matBillZ, matScale;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixIdentity(&matScale);
	matView._41 = m_pTransformCom->Get_Pos().x;
	matView._42 = m_pTransformCom->Get_Pos().y;
	matView._43 = m_pTransformCom->Get_Pos().z;

	D3DXMatrixScaling(&matScale, 0.2, 0.2, 0.2);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	m_pTransformCom->Set_WorldMatrix(&(matScale * matView));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 아이템 번호에 따라 texture번호 바꾸기
	m_pTextureCom->Set_Texture(m_iNum);

	m_pBufferCom->Render_Buffer();
}

void CItem::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
	if (pGameObject->Get_ObjType() == OT_PLAYER)
	{
		m_bDead = true;
	}
}

HRESULT CItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	switch (m_eItemtype)
	{
	case ITEM_INIT:
		return E_FAIL;
	case BULLET:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Bullet"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Bullet", pComponent });
		break;
	case EXP:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Exp"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Exp", pComponent });
		break;
	case HEALTH:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Health"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Health", pComponent });
		break;
	case KEY:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Key"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Key", pComponent });
		break;
	case MANA:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Mana"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Mana", pComponent });
		break;
	}

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	
	pComponent  = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });
	
	return S_OK;
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, ITEMTYPE eItemType, _int iNum)
{
	CItem* pInstance = DBG_NEW CItem(pGraphicDev, vPos, eItemType, iNum);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Item Create Failed");
		return nullptr;
	}

	return pInstance;
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	ITEMTYPE eType = ITEMTYPE(rand() % 5 + 1);
	_int iNum(0);
	switch (eType)
	{
	case BULLET:
		iNum = rand() % 4;
		break;
	case EXP:
		iNum = rand() % 3;
		break;
	case HEALTH:
		iNum = rand() % 2;
		break;
	case MANA:
		iNum = rand() % 2;
		break;
	default:
		eType = BULLET; //걍 예외처리함
		iNum = rand() % 4;
		break;
	}
	CItem* pInstance = DBG_NEW CItem(pGraphicDev, vPos, eType, iNum);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Item Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();
}