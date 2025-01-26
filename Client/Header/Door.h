#pragma once

#include "Base.h"
#include "InteractiveProp.h"
//TODO : 문 타입마다 텍스처 바꾸는 거 + 키 보유 정보 받아와서 언락시키기
class CDoor : public CInteractiveProp
{
public:
    explicit CDoor(const CDoor& rhs);

private:
    explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID);
    virtual ~CDoor();


public:
    static CDoor* Create(LPDIRECT3DDEVICE9   pGraphicDev, DOORID eDoorID);
    static CDoor* Create(LPDIRECT3DDEVICE9   pGraphicDev, DOORID eDoorID, D3DXVECTOR3 vPos, float fYAngle, SpawnGroup eSpawnGroup);
    void   Set_Elevate() { m_bElevate = true; }

    virtual void    Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
    virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

private:
    virtual HRESULT Ready_GameObject()                   override;
    HRESULT Ready_GameObject(D3DXVECTOR3 vPos, float fYAngle, SpawnGroup eSpawnGroup);
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
    virtual void Render_GameObject()                   override;
    virtual void Free()                               override;

    virtual  HRESULT   Add_Component()                  override;
    virtual void      TexNamer()                     override;

private:
    bool Elevate(const _float& fTimeDelta);      // 문 상승시키는 함수
    void Detect_Player(); //플레이어 거리 계산해서 Elevate On/Off 판단, 추후 충돌처리로 대체

private:
    DOORID m_eDoorID;
    float m_fSpeed; // 올라가는 속도값
    bool m_bElevate; //true되면 위로 올라감
    bool m_bDoorOn;
    bool m_bIsZDoor = false;
    SpawnGroup m_eSpawnGroup = SPAWN_END;
};
