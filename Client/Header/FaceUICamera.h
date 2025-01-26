#pragma once

#include "UICamera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CFaceUICamera : public Engine::CUICamera
{
private:
	explicit CFaceUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer);
	explicit CFaceUICamera(const CFaceUICamera& rhs);
	virtual ~CFaceUICamera();

public:
	HRESULT Ready_GameObject(
		const _float& fWidth,
		const _float& fHeight,
		const _float& fPosX,
		const _float& fPosY);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
	virtual void Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_float				m_fFrame = 0.f;
	_float m_fX;
	_float m_fY;
	CPlayer* m_pPlayer;

public:
	static CFaceUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _float& fWidth,
		const _float& fHeight,
		const _float& fPosX,
		const _float& fPosY,
		CPlayer*& pPlayer
	);

private:
	virtual void Free();
};