#pragma once

#include "Scene.h"

class CPlayer;
class CExit;


class CBossStage : public Engine::CScene
{
private:
	explicit CBossStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossStage();

public:
	HRESULT Ready_Scene(CPlayer& player);
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(const float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:

	HRESULT			Ready_Layer_Environment(wstring pLayerTag);
	HRESULT			Ready_Layer_GameLogic(wstring pLayerTag, CPlayer& player);
	HRESULT			Ready_Layer_UI(wstring pLayerTag);
	HRESULT			Ready_LightInfo();



private:
	CPlayer*		m_pPlayer;
	bool			m_teleport;
	_bool			m_bPhase2 = false;

public:
	static CBossStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer& player);

private:
	virtual void Free() override;

};

