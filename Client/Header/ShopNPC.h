#pragma once

#include "Base.h"
#include "InteractiveProp.h"
//TODO : �� Ÿ�Ը��� �ؽ�ó �ٲٴ� �� + Ű ���� ���� �޾ƿͼ� �����Ű��
class CShopNPC : public CInteractiveProp
{
private:
    explicit CShopNPC(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CShopNPC(const CShopNPC& rhs);
    virtual ~CShopNPC();

public:
    virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
    virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
    static CShopNPC* Create(LPDIRECT3DDEVICE9   pGraphicDev);

private:
    virtual HRESULT Ready_GameObject()                   override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
    virtual void Render_GameObject()                   override;
    virtual void Free()                               override;

    virtual  HRESULT   Add_Component()                  override;
    virtual void      TexNamer()                     override;  //�Ⱦ�

private:
    void Detect_Player(); //�÷��̾� �Ÿ� ����ؼ� Elevate On/Off �Ǵ�, ���� �浹ó���� ��ü
private:
    _bool m_bShopOn;
};
