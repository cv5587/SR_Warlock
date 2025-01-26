#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CSpawnToggle :
    public Engine::CGameObject
{
public:
    explicit CSpawnToggle(const CSpawnToggle& rhs);

protected:
    explicit CSpawnToggle(LPDIRECT3DDEVICE9 pGraphicDev, SpawnGroup eSpawnGroup);
    virtual ~CSpawnToggle();

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
    SpawnGroup m_eSpawnGroup = SPAWN_NONE;

public:
    static CSpawnToggle* Create(LPDIRECT3DDEVICE9 pGraphicDev, SpawnGroup eSpawnGroup, D3DXVECTOR3 vPos, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);

protected:
    virtual void Free();
};

