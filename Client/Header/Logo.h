#pragma once

#include "Scene.h"

#include "Loading.h"

class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(const float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT			Ready_Prototype();

	HRESULT			Ready_Layer_Environment(wstring pLayerTag);
	HRESULT			Ready_Layer_GameLogic(wstring pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(wstring pLayerTag) { return S_OK; }

private:
	CLoading*		m_pLoading;

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

