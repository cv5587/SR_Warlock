#pragma once

#include "GameObject.h"
BEGIN(Engine)

class CCamera;
class CUICamera;


class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup();

public:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_AlphaTest(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev);

public:
	void		Set_Camera(CCamera* pCamera) { m_pCamera = pCamera; }
	void		Set_UI_Camera(CUICamera* pUICamera) { m_pUICamera = pUICamera; }

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];

	CCamera* m_pCamera;
	CUICamera* m_pUICamera;
private:
	virtual void		Free();
};

END