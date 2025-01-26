#include "..\..\Header\Renderer.h"

#include "Camera.h"
#include "UICamera.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer() : m_pCamera(nullptr), m_pUICamera(nullptr)
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	if (RENDER_END <= eID || nullptr == pGameObject)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (!m_pCamera && !m_pUICamera)
	{
		Render_Priority(pGraphicDev);
		Render_NonAlpha(pGraphicDev);
		Render_AlphaTest(pGraphicDev);
		Render_Alpha(pGraphicDev);
		Render_UI(pGraphicDev);
		Clear_RenderGroup();
	}
	else
	{
		m_pCamera->Set_View_Space();
		m_pCamera->Set_Projection_Space();

		Render_Priority(pGraphicDev);
		Render_NonAlpha(pGraphicDev);
		Render_AlphaTest(pGraphicDev);
		Render_Alpha(pGraphicDev);
		Render_Particle(pGraphicDev);

		_matrix matWorld;

		Render_UI(pGraphicDev);

		m_pCamera->Set_View_Space();
		m_pCamera->Set_Projection_Space();

		Clear_RenderGroup();
	}

}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//(RGB)A		 (R`G`B`)A`
	//(AR, AG, AB) + ((1 - A)R`, (1 - A)G`, (1 - A)B`)

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
	{
		iter->Render_GameObject();
	}
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_AlphaTest(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xf0);
	for (auto& iter : m_RenderGroup[RENDER_ALPHATEST])
		iter->Render_GameObject();


	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_RenderGroup[RENDER_UI].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return static_cast<CUICamera*>(pDst)->m_iSortOrder <
				static_cast<CUICamera*>(pSrc)->m_iSortOrder;
		});

	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		iter->Render_GameObject();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRenderer::Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	for (auto& iter : m_RenderGroup[RENDER_PARTICLE])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
}


void CRenderer::Free()
{
	Clear_RenderGroup();
}