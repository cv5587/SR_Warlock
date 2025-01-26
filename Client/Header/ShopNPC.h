#pragma once

#include "Base.h"
#include "InteractiveProp.h"
//TODO : 문 타입마다 텍스처 바꾸는 거 + 키 보유 정보 받아와서 언락시키기
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
    virtual void      TexNamer()                     override;  //안씀

private:
    void Detect_Player(); //플레이어 거리 계산해서 Elevate On/Off 판단, 추후 충돌처리로 대체
private:
    _bool m_bShopOn;
};
