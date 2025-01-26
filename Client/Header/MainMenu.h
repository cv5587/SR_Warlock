#pragma once
#include "Scene.h"
#include "Loading.h"
#include "Export_Utility.h"
#include "UI.h"
#include "UITex.h"
#include "UIDynamicTex.h"

class CMainMenu : public Engine::CScene
{
private:
	explicit CMainMenu(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainMenu();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(const float& fTimeDelta) override;
	// CScene을(를) 통해 상속됨
	void Render_Scene() override;

private:
	HRESULT			Ready_Prototype();

	HRESULT			Ready_Layer_Environment(wstring pLayerTag);
	HRESULT			Ready_Layer_GameLogic(wstring pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(wstring pLayerTag) { return S_OK; }

private:
	CLoading* m_pLoading;
	CUI* m_UIGameStart;
	CUI* m_UIExit;

	CUITex* m_UITexImage1;
	CUIDynamicTex* m_UIDynamicTexImage1;
	CUIDynamicTex* m_UIDynamicTexImage2;
	CUIDynamicTex* m_UIDynamicTexImage3;
	bool m_bGameStartBtn;
	bool m_bExitBtn;
public:
	static CMainMenu* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	

private:
	virtual void Free() override;

};

