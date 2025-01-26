#pragma once

#include "UICamera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Player.h"
#include <ShopUICamera.h>

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CCursor : public Engine::CUICamera
{
private:
	explicit CCursor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCursor(const CCursor& rhs);
	virtual ~CCursor();

public:
	HRESULT Ready_GameObject(CShopUICamera* _pShopUI);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
	virtual void Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	CShopUICamera* m_pShopUI;

public:
	static CCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev, CShopUICamera* _pShopUI);

private:
	virtual void Free();
};