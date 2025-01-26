#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CLight;

END

class CLightToggle :
    public Engine::CGameObject
{
public:
    explicit CLightToggle(const CLightToggle& rhs);

protected:
    explicit CLightToggle(LPDIRECT3DDEVICE9 pGraphicDev, vector<CLight*> vecLightGroup);
    virtual ~CLightToggle();

public:
    HRESULT Ready_GameObject(D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
    virtual void Render_GameObject()						 override;

    virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
    virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

private:
    HRESULT			Add_Component();

private:
    Engine::CTransform* m_pTransformCom;

private:
    vector<CLight*> m_vecLightIdxGroup;
    bool            m_bIsCollide = false;
    bool            m_bIsFirstTime = true;
    
    float           m_fElipsedTime = 0.f;

public:
    static CLightToggle* Create(LPDIRECT3DDEVICE9 pGraphicDev, vector<CLight*> vecLightGroup, D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);

protected:
    virtual void Free();
};

