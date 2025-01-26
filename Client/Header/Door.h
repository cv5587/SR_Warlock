#pragma once

#include "Base.h"
#include "InteractiveProp.h"
//TODO : �� Ÿ�Ը��� �ؽ�ó �ٲٴ� �� + Ű ���� ���� �޾ƿͼ� �����Ű��
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
    bool Elevate(const _float& fTimeDelta);      // �� ��½�Ű�� �Լ�
    void Detect_Player(); //�÷��̾� �Ÿ� ����ؼ� Elevate On/Off �Ǵ�, ���� �浹ó���� ��ü

private:
    DOORID m_eDoorID;
    float m_fSpeed; // �ö󰡴� �ӵ���
    bool m_bElevate; //true�Ǹ� ���� �ö�
    bool m_bDoorOn;
    bool m_bIsZDoor = false;
    SpawnGroup m_eSpawnGroup = SPAWN_END;
};
