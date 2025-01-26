#include "Export_Utility.h"
#include <string>
#include <vector>

CComponent * Get_Component(COMPONENTID eID, wstring pLayerTag, wstring pObjTag, wstring pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* Get_GameObject(wstring pLayerTag, wstring pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

inline CLayer* Get_Layer(wstring pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

HRESULT Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagementInstance)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagementInstance = pManagement;

	return S_OK;
}

HRESULT Set_Scene(CScene * pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int Update_Scene(const _float & fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

void LateUpdate_Scene(const _float& fTimeDelta)
{
	CManagement::GetInstance()->LateUpdate_Scene(fTimeDelta);
}

void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

int Get_SceneIdx()
{
	return CManagement::GetInstance()->Get_SceneIdx();
}

HRESULT			Ready_Proto(const wstring pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent*		Clone_Proto(const wstring pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

// Renderer
void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

void     Set_Camera(CCamera* pCamera)
{
	CRenderer::GetInstance()->Set_Camera(pCamera);
}

void     Set_UI_Camera(CUICamera* pUICamera)
{
	CRenderer::GetInstance()->Set_UI_Camera(pUICamera);
}



wstring s2ws(const string& s)
{
	_int len;
	_int slength = static_cast<_int>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring ret(buf);
	delete[] buf;
	return ret;
}

string ws2s(const wstring& s)
{
	_int len;
	_int slength = static_cast<_int>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

float distanceToLineSegment(D3DXVECTOR2 p, D3DXVECTOR2 linePoint1, D3DXVECTOR2 linePoint2)
{
	D3DXVECTOR2 dir = linePoint2 - linePoint1;
	D3DXVECTOR2 projPoint = p - D3DXVec2Dot(&dir, &(p - linePoint1)) * dir / D3DXVec2Dot(&dir, &dir);
	bool isOnSegment = (projPoint.x >= min(linePoint1.x, linePoint2.x) && projPoint.x <= max(linePoint1.x, linePoint2.x)) &&
		(projPoint.y >= min(linePoint1.y, linePoint2.y) && projPoint.y <= max(linePoint1.y, linePoint2.y));

	if (isOnSegment)
	{
		return D3DXVec2LengthSq(&(p - projPoint));
	}

	return min(D3DXVec2LengthSq(&(p - linePoint1)), D3DXVec2LengthSq(&(p - linePoint2)));
}


//CollisionMgr

bool overlaps(CGameObject* pGameObject1, CGameObject* pGameObject2)
{
	CCollisionMgr::GetInstance()->overlaps(pGameObject1, pGameObject2);
}

void LateUpdate_CollisionMgr(float fTimeDelta)
{
	CCollisionMgr::GetInstance()->LateUpdate_CollisionMgr(fTimeDelta);
}

unsigned int Add_Collider(CGameObject* pGameObject)
{
	return CCollisionMgr::GetInstance()->Add_Collider(pGameObject);
}

unsigned int Add_Collider(CGameObject* pGameObject, double radius)
{
	return CCollisionMgr::GetInstance()->Add_Collider(pGameObject,radius);
}
unsigned int Add_Collider(CGameObject* pGameObject, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	return CCollisionMgr::GetInstance()->Add_Collider(pGameObject, lowerBound,upperBound);
}

HRESULT Remove_Collider(unsigned int particleIdx)
{
	return CCollisionMgr::GetInstance()->Remove_Collider(particleIdx);
}

HRESULT Render_Colliders()
{
	return CCollisionMgr::GetInstance()->Render_Colliders();
}


unsigned int Add_Line_Collider(CLine* pComponent, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	return CCollisionMgr::GetInstance()->Add_Line_Collider(pComponent, lowerBound, upperBound);
}
HRESULT Remove_LineCollider(unsigned int lineParticleIdx)
{
	return CCollisionMgr::GetInstance()->Remove_LineCollider(lineParticleIdx);
}

static CLineMgr* Create_LineMgr(LPDIRECT3DDEVICE9 pGraphicDev, std::vector<unsigned int> vlineGroup)
{
	return CLineMgr::Create(pGraphicDev, vlineGroup);
}


HRESULT      Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

HRESULT      Ready_Material(D3DMATERIAL9* pMaterialInfo, const MATERIALID& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Material(pMaterialInfo, iIndex);
}

inline _int Enable_Light(LPDIRECT3DDEVICE9 pGraphicDev, const LIGHTID& eLightID, const _uint& iChannelID, const _bool& bEnable, const _vec3& vPos, const _float& fRange)
{
	return CLightMgr::GetInstance()->Enable_Light(pGraphicDev, eLightID, iChannelID, bEnable, vPos, fRange);
}

void Enable_Material(LPDIRECT3DDEVICE9 pGraphicDev, const MATERIALID& iIndex)
{
	return CLightMgr::GetInstance()->Enable_Material(pGraphicDev, iIndex);
}

inline D3DLIGHT9 Get_Light(const LIGHTID& eLightID)
{
	return CLightMgr::GetInstance()->Get_Light(eLightID);

}

inline void Set_Light(const LIGHTID& eLightID, const D3DLIGHT9 eLight)
{
	return CLightMgr::GetInstance()->Set_Light(eLightID, eLight);
}

inline void Clear_Light(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return CLightMgr::GetInstance()->Clear_Light(pGraphicDev);
}


void Release_Utility()
{
	CManagement::DestroyInstance();
	CLightMgr::DestroyInstance();
	CRenderer::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CCollisionMgr::DestroyInstance();
}
