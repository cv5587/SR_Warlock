#pragma once

#include "Management.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "LightMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Texture.h"
#include "Mesh.h"

#include "Transform.h"
#include "Camera.h"
#include "UICamera.h"
#include "Calculator.h"
#include "FBXLoader.h"
#include "CollisionMgr.h"
#include "LineMgr.h"
#include "ParticleBuffer.h"
#include "DynamicRcTex.h"

#include "Line.h"


BEGIN(Engine)

// Management

inline CComponent*		Get_Component(COMPONENTID eID,wstring pLayerTag,wstring pObjTag,wstring pComponentTag);
inline CGameObject*		Get_GameObject(wstring pLayerTag, wstring pObjTag);
inline CLayer*			Get_Layer(wstring pLayerTag);
inline HRESULT			Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementInstance);

inline HRESULT			Set_Scene(CScene* pScene);
inline _int				Update_Scene(const _float& fTimeDelta);
inline void				LateUpdate_Scene(const _float& fTimeDelta);
inline void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
inline int				Get_SceneIdx();

// ProtoMgr
inline HRESULT			Ready_Proto(const wstring pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_Proto(const wstring pProtoTag);

inline static CLineMgr* Create_LineMgr(LPDIRECT3DDEVICE9 pGraphicDev, std::vector<unsigned int> vlineGroup);

// Renderer
inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup();

inline void     Set_Camera(CCamera* pCamera);
inline void     Set_UI_Camera(CUICamera* pUICamera);

// LightMgr
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline HRESULT		Ready_Material(D3DMATERIAL9* pMaterialInfo, const MATERIALID& iIndex = MTRL_DEFAULT);
inline _int			Enable_Light(LPDIRECT3DDEVICE9 pGraphicDev, const LIGHTID& eLightID = LIGHT_DEFAULT, const _uint& iChannelID = 0,
	const _bool& bEnable = true, const _vec3& vPos = { 0.f, 0.f, 0.f }, const _float& fRange = -1.f);
inline void			Enable_Material(LPDIRECT3DDEVICE9 pGraphicDev, const MATERIALID& iIndex = MTRL_DEFAULT);
inline D3DLIGHT9	Get_Light(const LIGHTID& eLightID);
inline void			Set_Light(const LIGHTID& eLightID, const D3DLIGHT9 eLight);
inline void			Clear_Light(LPDIRECT3DDEVICE9 pGraphicDev);

//CollisionMgr
inline bool overlaps(CGameObject* pGameObject1, CGameObject* pGameObject2);

inline void LateUpdate_CollisionMgr(float fTimeDelta);

inline unsigned int Add_Collider(CGameObject* pGameObject);
inline unsigned int Add_Collider(CGameObject* pGameObject, double radius);
inline unsigned int Add_Collider(CGameObject* pGameObject, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);
inline HRESULT Remove_Collider(unsigned int particleIdx);

inline unsigned int Add_Line_Collider(CLine* pComponent, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);
inline HRESULT Remove_LineCollider(unsigned int lineParticleIdx);

inline HRESULT Render_Colliders();

//string utils
inline wstring s2ws(const string& s);
inline string ws2s(const wstring& s);

//distance utils
inline float distanceToLineSegment(D3DXVECTOR2 p, D3DXVECTOR2 linePoint1, D3DXVECTOR2 linePoint2);

// Release
inline void				Release_Utility();


#include "Export_Utility.inl"

END