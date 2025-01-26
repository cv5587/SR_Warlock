#pragma once

#include "Scene.h"

class CPlayer;
class CExit;

BEGIN(ENGINE)

class CFBXLoader;

END

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(const float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:

	HRESULT			Ready_Layer_Environment(wstring pLayerTag);
	HRESULT			Ready_Layer_GameLogic(wstring pLayerTag);
	HRESULT			Ready_Layer_UI(wstring pLayerTag);
	HRESULT			Ready_LightInfo();



private:
	CPlayer*		m_pPlayer;
	CExit*			m_pExit;
	bool			m_teleport;
	_bool			m_bPhase2 = false;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

